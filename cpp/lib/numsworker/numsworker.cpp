#include "numsworker.hpp"
#include "ThreadSafeQueue.hpp"
#include "dataPacket.hpp"
#include "dataElement.hpp"
#include "nums_constants.hpp"
#include "TcpClient.hpp"
#include <unordered_map>
#include <array>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstddef>
#include <cerrno>
#include <arpa/inet.h>
#include <thread>

numsworker::numsworker(ThreadSafeQueue<nums::Packet>& outq,
                       ThreadSafeQueue<nums::Packet>& inq)
    : outq_(outq),
      inq_(inq)
{}

numsworker::~numsworker(){
    stop();
}

void numsworker::stop(){
    outq_thread_.request_stop(); //NUMS->SMSC
}

void numsworker::start(){
    outq_thread_ = std::jthread([this](std::stop_token st) {
        outq_monitor(st);
    });
}

void numsworker::outq_monitor(std::stop_token st){ // NUMS->SMSC (1,3) NUMS<-SMSC(2,4,5), NUMS->SMSC(6)
    spdlog::info("[NUMS->SMSC]");
    while (!st.stop_requested()) {
        while (!smsc_.connect("127.0.0.1", 8500)){ //112.172.129.233
            if (st.stop_requested()) return;
            spdlog::error("[NUMS->SMSC] connect failed, retry...");
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
        spdlog::info("[NUMS->SMSC] Connected.");
        while (auto msg = outq_.pop_wait(st)) { // type: Packet (JSON parsing)
            // pop_wait에 stop_token 넘겨서 사용하게함, stop 요청 오면 nullopt 반환하고 종료
            spdlog::info("[NUMS->SMSC]:{}", msg->toString());
            
            std::vector<std::byte> out(msg->total_size()); // thread_local buffer(고성능 서버에서 사용)
            msg->serialize(out.data(), out.size()); // packet 데이터를 buffer에 put
            
            if (!smsc_.send_all(out.data(), out.size())) {
                spdlog::error("[NUMS->SMSC] send failed.");
                std::this_thread::sleep_for(std::chrono::seconds(5));
                break;
            }
            spdlog::info("[NUMS->SMSC] sent:{}\n", msg->toString());
            spdlog::info("(info) send size={}", out.size());
            spdlog::info("(info) packet total={}", msg->total_size());

            //응답에 따라 달라져야함-> Header, Body 처리 따로
            //smsc로부터 오는 유형 : 1->2, 3->4&5->6send
            bool reportchk = false;
            std::vector<std::byte> header_buf(nums::Header::SIZE);
            while(!reportchk){
                if (!smsc_.read_exact(header_buf.data(), header_buf.size())) {
                    spdlog::error("[NUMS]read header failed\n");
                    break;
                }
                //ACK 안오면 재전송(?)
                if (!smsc_.send_all(out.data(), out.size())) {
                    spdlog::error("[NUMS->SMSC] No Ack, close connection");
                    smsc_.close();
                    break;
                }
                nums::Header rep_h{};
                rep_h.deserialize(header_buf.data());
                spdlog::info("[SMSC->NUMS] received(Header):{}\n", rep_h.toString());
                
                // 1) rep_h.get_msg_type() 에 따라 body_buf 정의
                size_t body_size = 0;
                reportchk = false; //MsgType::REQ_RESULT은 report 메시지가 오는 것임.
                switch(rep_h.msg_type_enum()){
                    case MsgType::SN_REGINFO_ACK: body_size = 4;  break;
                    case MsgType::REQ_RESULT: body_size = 4; reportchk = true; break;
                    default: body_size = 0;
                }
                if(body_size == 0) { //위 두 case 아니면 LINK ACK 이므로 while문 탈출
                    spdlog::info("[SMSC->NUMS]LINK ACK.");
                    //body 없으니 body set X
                    nums::Packet result{};
                    result.header = rep_h;
                    inq_.push_noti(result);
                    break;
                }
                //위 switch문에서 case 걸린 경우만 다음 로직 수행
                std::vector<std::byte> body_buf(body_size);
                if (!smsc_.read_exact(body_buf.data(), body_buf.size())) {
                    spdlog::error("[NUMS]read body failed\n");
                    break;
                }
                nums::result_body res_b{};
                res_b.deserialize(body_buf.data());
                
                spdlog::info("[SMSC->NUMS] received(Body):{}\n", res_b.toString());
                if(reportchk){ //SN_REGINFO_ACK인 경우 이 로직 수행 안하고 while 한번 더 돈다
                    //nums::Packet 객체 만들어서 inq에 push_noti()
                    // HEADER 은 rep_h를 사용. msg_type=6으로 재설정. BODY는 E_OK 담으면됨?
                    //smsc로 ACK send ()
                    //그럼 smsc가 연결 안 끊겠지
                    //근데 끊겼고 health chker가 다시 연결수립해줬어. 근데 블로킹같은 경우는 이대로 메시지 유실 아님?
                    nums::Packet result{};
                    result.header = rep_h;
                    result.body = res_b;
                    inq_.push_noti(result);
                    spdlog::info("report arrived.");
                }
            }
        }
    }
}



bool send_all(int fd, const std::byte* data, size_t len) {
    size_t sent = 0;
    while (sent < len) {
        ssize_t n = ::send(fd, data + sent, len - sent, 0);
        if (n <= 0) return false;
        sent += n;
    }
    return true;
}


//std::array<std::byte, msg->total_size()> in_buf{}; //컴파일 타임에 크기가 정해진다
