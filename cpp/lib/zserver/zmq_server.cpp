#include "zmq_server.hpp"
#include "ThreadSafeQueue.hpp"
#include "dataPacket.hpp"
#include "dataElement.hpp"
#include "nums_constants.hpp"
#include <unordered_map>
#include <array>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstddef>
#include <cerrno>
#include <arpa/inet.h>

zmq_server::zmq_server(ThreadSafeQueue<nums::Packet>& outq,
                       ThreadSafeQueue<nums::Packet>& inq)
    : outq_(outq),
      inq_(inq)
{}

zmq_server::~zmq_server()
{ stop(); }

void zmq_server::start()
{
    spdlog::info("ZMQ Server up.");

    sock_.set(zmq::sockopt::linger, 0);
    sock_.set(zmq::sockopt::rcvhwm, 30000);
    sock_.set(zmq::sockopt::rcvtimeo, 100); //recv 최대 100ms blocking

    monitor_.init(sock_, "inproc://zmqsvr-monitor");
    sock_.bind("tcp://*:9984");

    mon_th_ = std::jthread([this](std::stop_token st) {
        while (!st.stop_requested()) {
            (void)monitor_.check_event(100);
        }
    });
    in_handler_th_ = std::jthread([this](std::stop_token st) {
        in_handler(st);
    });

    out_handler_th_ = std::jthread([this](std::stop_token st) {
        out_handler(st);
    });
}

void zmq_server::stop()
{ //소멸 순서
    out_handler_th_.request_stop();
    in_handler_th_.request_stop();
    mon_th_.request_stop();
}

void zmq_server::in_handler(std::stop_token st) {
    // ZMQ 소켓 event 감지(poll) -> NUMS
    while (!st.stop_requested()) {
        // zmq::pollitem_t items[] = {
        //     { static_cast<void*>(sock_), 0, ZMQ_POLLIN, 0 }
        // };
        // zmq::poll(items, 1, 100); // (socket 1개, 100ms wake&loop)
        // if (items[0].revents & ZMQ_POLLIN) {
            std::vector<zmq::message_t> recv_frames;
            do{
                zmq::message_t msg;
                sock_.recv(msg, zmq::recv_flags::none); //blocking인데 poll이후라 any recv_flags
                recv_frames.push_back(std::move(msg)); 
            } while (sock_.get(zmq::sockopt::rcvmore));

            if (!recv_frames.empty()){
                auto& payload_msg = recv_frames.back();
                auto p = nums::Packet::parse_json(
                    static_cast<const std::byte*>(payload_msg.data()),
                    payload_msg.size()
                );
                p.set_identity(std::move(recv_frames[0])); 
                outq_.push_noti(std::move(p)); 
            }
        // }
        //ROUTER
        while(auto reply = inq_.try_pop(st)){ //try_pop (없으면 계속 null 반환) > notify 기반 blocking pop(O)
            auto j = reply->to_json();
            std::string payload = j.dump();

            sock_.send(reply->get_identity(), zmq::send_flags::sndmore);
            sock_.send(zmq::message_t(0), zmq::send_flags::sndmore);

            zmq::message_t msg(payload.size());
            std::memcpy(msg.data(), payload.data(), payload.size());
            sock_.send(msg, zmq::send_flags::none);
        }
    }
}





// 하나의 ZMQ socket은 하나의 스레드에서만 사용 -이유: zmq 소켓은 thread-safe 하지 않음
// 여러 스레드에서 동시에 접근 x(데이터 레이스, 메시지 손실, UB..)
// 따라서 각 소켓은 단일 스레드에서만 사용해야 함. 
// 그럼 소켓 여러개 쓰는 경우는 뭐지?

//identity 저장-[0]:주소, [1]:empty delimeter, back():payload(JSON)

//========================밑부터는 REQ/REP, 동기 야매논블로킹========================

//여기서 한번씩 socket read? 양방향 health check를 위해서?
//다음 recv 전에 send 필요 (REQ REP 모델)
//inq blocking pop

/*
const auto size = reply->total_size();
zmq::message_t msg(size);
reply->serialize(
    static_cast<std::byte*>(msg.data()),
    size
);
sock_.send(msg, zmq::send_flags::none);
*/

/* outq_ MONITORING (main 함수에 있던 녀석)
std::thread t_outq_([&] { // MONITOR(outq_)
        while (true) {
            if (auto msg = svr.outq_.pop()) {
                spdlog::info("[NUMS] outq - RES");
            }
        }
    });
*/

/* stop_ 이 뭐였는지 알아내셈 (지금은 jthread로 바꾼 상태)
    while (!stop_.load()) {
        if (auto msg = outq_.pop()) { 
            spdlog::info("[NUMS] outq - RES");
        }
    }
    */

/* std::string VS std::string_view
   std::string s(
        static_cast<const char*>(msg.data()), //raw bytes->nums::Packet
        msg.size()
    );
*/    

/*
outq_thread_ = std::jthread(&zmq_server::outq_monitor, this);
    inq_thread_ = std::jthread(&zmq_server::inq_monitor, this);

*/

// 밑 뭔말?
// 메시지 없으면 막히는 걸 피하려면 timeout/poll을 써야 함 
// 여기선 poll로 100ms마다 깨워서 stop 체크
// -1(무한대기) 대신 timeout으로 돌려야 stop 가능


//분기 필요 - zmq_client 에서 온 JSON인지 SMSC에서 온 raw bytes 인지
// -> nums::Packet::parse_json / nums::Packet::parse_bytes
//ㄴㄴ 분기 필요없음

// nums::Packet& p = *msg; // 또는 msg.value()


// Packet.header.seq_num 부여하고 완성된 Packet.header를 try_cnt,send_time과 함께 
            // retry_map(unordered_map)에 put
            // & SMSC로 send할 패킷 생성 및 send

            // SFUNC(smsc 전송 && seq_num+=1) && retry_map에 put(send_time, seq_num)
            // retry_map 에 push
            // packet 생성 후 smsc로 send
            // 기다림..
            // 안오면 -> 재전송,retry_map update
            // 오면 -> packet을 객체화 -> outq_.push()


/*
zmq_cli_monitor 함수 안에 있던 
//밑 5줄 용도 : 소켓 여러개 감시하는 멀티플렉서에서 필요한 것. 지금은 단일소켓 처리
        // zmq::pollitem_t items[] = { // 논블로킹 recv. 여러 socket 이벤트 처리
        //     { sock_, 0, ZMQ_POLLIN, 0 }
        // };
        // zmq::poll(items, 1, std::chrono::milliseconds(100));
        // if (!(items[0].revents & ZMQ_POLLIN)) continue;


*/


// std::move - 이후 기존객체 값 다시 읽으면 안됨, const 객체에 사용 불가
