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
#include <chrono>
#include <string>
#include <cstdint>
#include <array>

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

void numsworker::outq_monitor(std::stop_token st){
    //zmq client -> sendMsg
    while (!st.stop_requested()) {
        while (!smsc_.connect(serv_host, serv_port)) {
            spdlog::error("Connect to SMSC failed, retry...");
            std::this_thread::sleep_for(1s);
        }
        spdlog::info("Connected to SMSC.");

        // recv 한번 하는 이유 :
        // 연결 수립 시 SMSC에서 LINK 메시지 보내는 경우 있어서?
        auto result = recvMsg();
        if(result){
            spdlog::info("Initial LINK received after connect.{}\n", result->toString());
        }

        auto msg_opt = outq_.pop_wait_for(st, 5s);
        nums::Packet msg;
        if (!msg_opt.has_value()) { // IDLE 상태면 LINK 생성(주기: 5초)
            msg = nums::Packet(MsgType::LINK);
        }else{
            msg = *msg_opt;
        }
        if(sendMsg(msg)){ // 1, 3 send
            auto result = recvMsg();
            if(result){ // 2, 5 recv
                inq_.push_noti(result);
            }
        }else{
            spdlog::error("Connection lost during send");
            smsc_.close(); // 연결 끊김 처리
        }
    }
}


bool numsworker::sendMsg(const nums::Packet& msg){
    std::vector<std::byte> out(msg.total_size());
    msg->serialize(out.data(), out.size()); 
    if (!smsc_.send_all(out.data(), out.size())) { // 1 or 3 전송
        spdlog::error("msg send failed.");
        return false;
    }
    spdlog::info("sent:{}\n", msg.toString());
    spdlog::info("send size={}", out.size());
    spdlog::info("packet total={}", msg.total_size());
    return true;
}

//seq 오류처리. 내가 보낸 메시지에 대한 옳지 못한 응답에 대한 오류처리
std::optional<nums::Packet> numsworker::recvMsg() {
    while(true){
        auto rep_h = recvHeader();
        if (!rep_h) return std::nullopt;
        switch(rep_h.msg_type_enum()){
            case MsgType::LINK:
                {
                    nums::Packet ack(MsgType::LINK_ACK);
                    sendMsg(ack);
                    spdlog::info("LINK received, sending ACK");
                    break;
                }
            case MsgType::LINK_ACK:
                // (내가 보낸 메시지에 대한 응답 오기전에 ACK가 오는 케이스가 없다고 가정)
                {
                    nums::Packet result{};
                    result.header = *rep_h;
                    spdlog::info("LINK ACK received.\n");
                    return result;
                }
            case MsgType::SN_REGINFO_ACK:
                {
                    auto rep_b = recvBody(*rep_h);
                    if (!rep_b) return std::nullopt; //실패 시 return
                    break;
                }
            case MsgType::REQ_RESULT:
                {
                    auto rep_b = recvBody(*rep_h);
                    if (!rep_b) return std::nullopt;
                    nums::Packet result{};
                    result.header = *rep_h;
                    result.body = *rep_b;
                    spdlog::info("report arrived.");
                    nums::Packet ack(MsgType::REQ_RESULT_ACK);
                    sendMsg(ack);
                    return result;
                }
            default:
                {return std::nullopt;}
        }
    }
}

std::optional<nums::Header> numsworker::recvHeader(){
    std::vector<std::byte> header_buf(nums::Header::SIZE); 
    if (!smsc_.read_exact(header_buf.data(), header_buf.size())) {
        spdlog::error("read header failed\n");
        return std::nullopt;
    }
    nums::Header rep_h{};
    rep_h.deserialize(header_buf.data());
    spdlog::info("Header received:{}\n", rep_h.toString());
    return rep_h; // 성공 시 객체 반환 (암시적으로 optional로 변환됨)
}

std::optional<nums::Body> numsworker::recvBody(nums::Header h){
    std::vector<std::byte> body_buf(h.get_msg_len());
    if (!smsc_.read_exact(body_buf.data(), body_buf.size())) {
        spdlog::error("[NUMS]read body failed\n"); // body 못읽음
        return std::nullopt;
    }
    nums::result_body b{};
    b.deserialize(body_buf.data());
    spdlog::info("Body received:{}\n", b.toString());
    return b;
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

// 참조자(const &) 사용 - Optional(std::optional<nums::Packet>) 대신 
//nullopt이 전달될 염려는 없지만 const nums::Packet& 로 함수 시그니처를 바꾸는 것이 안전&깔끔
//사이즈 고정되어있다면 vector 사용 이유 없음. array로 교체

// thread_local buffer(고성능 서버에서 사용)

/* 불필요한 헬스체크 스레드 deprecate
void numsworker::healthChkstart(){
    health_checker_thread_ = std::jthread([this](std::stop_token st) {
        health_chker(st);
    });
}

void numsworker::health_chker(std::stop_token st){ //상태 감지 ONLY
    using namespace std::chrono_literals;
    spdlog::info("[Health] started");
    std::atomic<bool> flag1;
    std::atomic<bool> flag2;
    std::atomic<bool> flag3;
    std::atomic<bool> flag4;
    while (!st.stop_requested()) {
        auto now = clock::now();
        auto last = last_ok_.load();
        flag1 = connected_.load();
        flag2 = !reconnect_requested_.load(); // flag 중복 방지
        flag3 = (now - last > 5s); //오고가는 메시지 없거나, IDLE 상태 -> 어케 구분할 거냐는 거임
        flag4 = !idle_mode.load(); //IDLE 모드 아닌 경우
        if (flag1 && flag2 && flag3 && flag4) {
            spdlog::warn("[Health] no successful I/O for 5s, request reconnect");
            reconnect_requested_.store(true);
        }
        std::this_thread::sleep_for(1s);
    }
    spdlog::info("[Health] stopped");
    //IDLE 상태일 때도 flag3에 의해 플래그 바꾼다니까 지금
}

void numsworker::outq_monitor(std::stop_token st){ 
    using namespace std::chrono_literals;
    spdlog::info("[NUMS->SMSC] started");
    while (!st.stop_requested()) { //start
        while (!smsc_.connect(serv_host, serv_port)) { // Host: "127.0.0.1"
            //연결 재수립
            if (st.stop_requested()) return; //?
            connected_.store(false);
            spdlog::error("[NUMS->SMSC] connect failed, retry...");
            std::this_thread::sleep_for(1s);
        }
        connected_.store(true);
        reconnect_requested_.store(false); 
        last_ok_.store(clock::now());
        spdlog::info("[NUMS->SMSC] Connected.");

        while (!st.stop_requested()) {
            // health_checker가 reconnect 요청했는지 확인
            if (reconnect_requested_.load()) { // vs exchange()
                spdlog::info("[NUMS->SMSC] reconnect requested");
                smsc_.close();
                connected_.store(false);
                reconnect_requested_.store(false); 
                break;
            } else {
                spdlog::info("[NUMS] reconnect_requested_ : false");
            }
            auto msg = outq_.pop_wait_for(st, 5s);// 5초만 기다림
            if (!msg.has_value()) {
                idle_mode.store(true);
                nums::Packet p(MsgType::LINK);
                msg = p;
            } else {
                idle_mode.store(false);
            }
            spdlog::info("[NUMS] idle_mode:{}", idle_mode.load());
            //recv();
            if(!send_and_recv(msg)) break; //연결 재수립하러 가셈
        }
    }
}

bool numsworker::send_and_recv(std::optional<nums::Packet> msg) {
    bool SUCFLAG = true;
    bool REPORTFLAG = false;

    std::vector<std::byte> out(msg->total_size()); // thread_local buffer(고성능 서버에서 사용)
    msg->serialize(out.data(), out.size()); // packet 데이터를 buffer에 put
    if (!smsc_.send_all(out.data(), out.size())) { //1번 OR 3번 보냄
        spdlog::error("[NUMS->SMSC] send failed.");
        smsc_.close();
        connected_.store(false);
        SUCFLAG = false;
        return SUCFLAG;
    }
    last_ok_.store(clock::now());
    spdlog::info("[NUMS->SMSC] sent:{}\n", msg->toString());
    spdlog::info("(info) send size={}", out.size());
    spdlog::info("(info) packet total={}", msg->total_size());
    //이 함수 한번 사용 후 다시 이 함수 사용시 여기서 다음 while loop로 또 들어감
    // 답 : 내가 임의로 만든 LINK 메시지와 zmq client 에서 오는 LINK 메시지 다르게 처리됨
    std::vector<std::byte> header_buf(nums::Header::SIZE);
    while(!REPORTFLAG){ //do while 
        if (!smsc_.read_exact(header_buf.data(), header_buf.size())) { //2or4인지 헤더읽음
            spdlog::error("[NUMS]read header failed\n"); // header 못읽음
            smsc_.close();
            connected_.store(false);
            SUCFLAG = false;
            break;
        }
        last_ok_.store(clock::now()); // 읽음
        
        nums::Header rep_h{}; //2or4 deserialize
        rep_h.deserialize(header_buf.data());
        last_ok_.store(clock::now());

        spdlog::info("[SMSC->NUMS] received(Header):{}\n", rep_h.toString());
        
        // size_t body_size = rep_h.get_msg_len();
        // bool LNKFLAG = body_size == 0 ? true : false;
        REPORTFLAG = rep_h.msg_type_enum() == MsgType::REQ_RESULT? true : false;
        
        if(rep_h.msg_type_enum() == MsgType::LINK) {
            spdlog::info("[SMSC->NUMS]LINK ACK.");
            nums::Packet result{};
            result.header = rep_h;
            inq_.push_noti(result);
            last_ok_.store(clock::now());
            break;
        }
        std::vector<std::byte> body_buf(rep_h.get_msg_len()); //4,5
        if (!smsc_.read_exact(body_buf.data(), body_buf.size())) {
            spdlog::error("[NUMS]read body failed\n"); // body못읽음
            smsc_.close();
            connected_.store(false);
            SUCFLAG = false;
            break;
        }
        nums::result_body res_b{};
        res_b.deserialize(body_buf.data());
        last_ok_.store(clock::now());
        spdlog::info("[SMSC->NUMS] received(Body):{}\n", res_b.toString());
        if(REPORTFLAG){ //SN_REGINFO_ACK인 경우 이 로직 수행 안하고 while 한번 더 돈다
            nums::Packet result{};
            result.header = rep_h;
            result.body = res_b;
            inq_.push_noti(result);
            spdlog::info("report arrived.");
            break;
        }
    }
    return SUCFLAG;
}
*/

//이거 아니고 return msg 해주면 안돼? outq_monitor에서 해당 메시지 다시 보내게
// 장점 : 모든 과정에서 선택한 메시지만 보낼 수 있음
// 문제점 : SUCCESS 일 때는 뭐 반환할 건데ㅋㅋ


//std::array<std::byte, msg->total_size()> in_buf{}; //컴파일 타임에 크기가 정해진다

// HEADER 은 rep_h를 사용. msg_type=6으로 재설정. BODY는 E_OK 담으면됨?
//smsc로 ACK send ()
//그럼 smsc가 연결 안 끊겠지
//근데 끊겼고 health chker가 다시 연결수립해줬어. 근데 블로킹같은 경우는 이대로 메시지 유실 아님?


// pop_wait에 stop_token 넘겨서 사용하게함, stop 요청 오면 nullopt 반환하고 종료
//응답에 따라 달라져야함-> Header, Body 처리 따로
//smsc로부터 오는 유형 : 1->2, 3->4&5->6send

// NUMS->SMSC (1,3) NUMS<-SMSC(2,4,5), NUMS->SMSC(6)

//내가 기대하는 응답/이벤트를 하나 처리할 때까지 돈다

//numsworker 객체의 멤버를 사용하려면 앞에 '객체::'를 붙여야하는구나

//std::this_thread::sleep_for(1s)->condition var, wait_for 로 리팩토링
