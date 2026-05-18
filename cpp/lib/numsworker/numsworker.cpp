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
//Boost
#include <boost/asio/io_context.hpp>

numsworker::numsworker(net::io_context& io,
            ThreadSafeQueue<nums::Packet>& outq,
            ThreadSafeQueue<nums::Packet>& inq)
    : io_(io),
      outq_(outq),
      inq_(inq)
{}

numsworker::~numsworker(){
    stop();
}

void numsworker::stop(){
    outq_thread_.request_stop(); // NUMS->SMSC
    // heartbeat_sender_.request_stop(); // heartbeat_sender_
}

void numsworker::start(){ //jthread -> io 등록으로 변경
    // heartbeat_sender_ = std::jthread([this](std::stop_token st) {
    //     heartbeat_sender(st);
    // });
}

void numsworker::handle_request(nums::Packet p){
    //smsc_.async_send
    //retry_map에 try_cnt=0, seq_num, send_time put
}

void numsworker::async_send(const nums::Packet& msg){
    //sendMsg() 호출
    //sendMsg() 성공 시 retry_map에 try_cnt=0, seq_num, send_time 기록
    if (sendMsg(msg)) {
        // retry_map에 넣음
    }
}


bool numsworker::sendMsg(const nums::Packet& msg){
    std::vector<std::byte> out(msg.total_size());
    msg.serialize(out.data(), out.size()); 
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
std::optional<nums::Packet> numsworker::recvMsg(const nums::Packet& sentmsg) {
    // using namespace std::chrono_literals;
    std::chrono::seconds timeout;
    switch(sentmsg.header.msg_type_enum()){ // type 에 따라 timeout 설정
        //sent 메시지 타입 1,3 ONLY (다른 것 올 수 없음)
        case MsgType::LINK: //1
            timeout = 10;
            break;
        case MsgType::SN_REGINFO: //3
            timeout = 5;
            break;
        default:
            timeout = 0;
    }
    while(true){
        auto rep_h = recvHeader(timeout);
        if (!rep_h) { //header recv 실패 시 재전송
            if(retry_cnt_ < 1){
                outq_.push_noti(sentmsg); // optional 객체였다면 * 로 꺼내야하는 게 맞고, 내 param은 const & 이니까 바로 전달 ok?
                retry_cnt_++;
            } else { //재전송 실패
                retry_cnt_ = 0; //초기화
                return std::nullopt; // std::optional<nums::Packet> 반환인데 이거 줘도 되나? 
            }
        }
        switch((*rep_h).msg_type_enum()){
            case MsgType::LINK: //SMSC에서 온 LNK
                { 
                    nums::Packet ack(MsgType::LINK_ACK);
                    outq_.push_noti(ack);
                    break;
                }
            case MsgType::LINK_ACK: //내가 보낸 메시지에 대한 응답 오기전에 ACK가 오는 케이스가 없다고 가정
                {
                    nums::Packet result{};
                    result.header = *rep_h;
                    return result;
                }
            case MsgType::SN_REGINFO_ACK:
                {
                    timeout = 5;
                    auto rep_b = recvBody(*rep_h, timeout);
                    if (!rep_b) return std::nullopt; //body recv 실패 시 재전송 X
                    break;
                }
            case MsgType::REQ_RESULT:
                {
                    timeout = 5;
                    auto rep_b = recvBody(*rep_h, timeout);
                    if (!rep_b) return std::nullopt; //body recv 실패 시 재전송 X

                    nums::Packet result{};
                    result.header = *rep_h;
                    result.body = *rep_b;
                    spdlog::info("report arrived.");
                    nums::Packet ack(MsgType::REQ_RESULT_ACK);
                    outq_.push_noti(ack); //outq_.push_noti(ack) vs sendMsg(ack);
                    return result;
                }
            default:
                {
                    spdlog::error("Unknown message type received.");
                    nums::Packet result{};
                    result.header = *rep_h;
                    result.body = {};
                    return result;
                }
        }
    }
}

std::optional<nums::Header> numsworker::recvHeader(std::optional<std::chrono::milliseconds> timeout){
    std::vector<std::byte> buf(nums::Header::SIZE); 
    if (!smsc_.read_exact(buf.data(), buf.size(), timeout)) {
        spdlog::error("read header failed\n");
        return std::nullopt;
    }
    nums::Header rep_h{};
    rep_h.deserialize(buf.data());
    spdlog::info("Header received:{}\n", rep_h.toString());
    return rep_h; // 성공 시 객체 반환 (암시적으로 optional로 변환됨)
}

std::optional<nums::Body> numsworker::recvBody(nums::Header h,std::optional<std::chrono::milliseconds> timeout){ 
    // 안전하게 recv timeout(5s) VS 0s
    std::vector<std::byte> buf(h.get_msg_len());
    if (!smsc_.read_exact(buf.data(), buf.size(), timeout)) { 
        spdlog::error("read body failed\n");
        return std::nullopt;
    }
    nums::result_body b{};
    b.deserialize(buf.data());
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

/*
void numsworker::heartbeat_sender(std::stop_token st){
    //5초 주기
    //connected_ 상태에 따라 LINK 메시지 outq_.push()
    using namespace std::chrono_literals;
    while (!st.stop_requested()) {
        if(connected_.load()){
            nums::Packet lnk;
            lnk = nums::Packet(MsgType::LINK);
            outq_.push_noti(std::move(lnk)); 
        }
        std::this_thread::sleep_for(5s);
    }
}
*/

/* 연결 확인 후 연결 재수립 - while
if(!connected_.load()){ //connected_ 초기값은 false고, 이에 따라 연결 재수립
    if(!smsc_.connect(serv_host, serv_port)){
        spdlog::error("Connect to SMSC failed, retry...");
        std::this_thread::sleep_for(1s);
        continue;
    }
    connected_.store(true);
    spdlog::info("Connected to SMSC.");
}
*/