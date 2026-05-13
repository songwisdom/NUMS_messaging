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

void zmq_server::start() //jthread 3개 -> io 등록으로 변경
{
    spdlog::info("ZMQ Server up.");
    //socket 옵션
    sock_.set(zmq::sockopt::linger, 0);
    sock_.set(zmq::sockopt::rcvhwm, 30000);
    sock_.set(zmq::sockopt::rcvtimeo, 100); //recv 최대 100ms blocking

    //모니터 함수
    monitor_.init(sock_, "inproc://zmqsvr-monitor");
    sock_.bind("tcp://*:9984");

    //스레드 띄우기
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

void zmq_server::in_handler(std::stop_token st) { //ZMQ -> NUMS -> SMSC
    // ZMQ 소켓 event 감지(poll) -> outq_에 패킷화해서 push
    while (!st.stop_requested()) {
        zmq::pollitem_t items[] = {
            { static_cast<void*>(sock_), 0, ZMQ_POLLIN, 0 }
        };
        zmq::poll(items, 1, 100ms); // (socket 1개, 100ms wake&loop)
        if (items[0].revents & ZMQ_POLLIN) {
            std::vector<zmq::message_t> recv_frames;
            do{
                zmq::message_t msg;
                sock_.recv(msg, zmq::recv_flags::none); 
                //recv (none옵션: blocking, dontwait: non-blocking-busy)
                recv_frames.emplace_back(std::move(msg)); 
            } while (sock_.get(zmq::sockopt::rcvmore));

            if (recv_frames.size() < 2) { //예외 처리
                spdlog::warn("invalid ROUTER message: frame_count={}", recv_frames.size());
            } else {
                auto& payload_msg = recv_frames.back();
                auto p = nums::Packet::parse_json(
                    static_cast<const std::byte*>(payload_msg.data()),
                    payload_msg.size()
                );
                p.set_identity(std::move(recv_frames[0])); 
                outq_.push_noti(std::move(p)); 
            }
        }
        //ROUTER
        while(auto reply = inq_.try_pop(st)){ //try_pop (없으면 계속 null 반환) > notify 기반 blocking pop(O)
            //다음 세 줄을 별도 스레드로 분리 
            auto r = reply->to_json();
            std::string payload = r.dump();
            auto id = reply->get_identity();

            sock_.send(
                zmq::buffer(id.data(), id.size()),
                zmq::send_flags::sndmore
            );
            sock_.send(
                zmq::message_t(0),
                zmq::send_flags::sndmore
            );

            sock_.send(
                zmq::buffer(payload.data(), payload.size()),
                zmq::send_flags::none
            );
        }
    }
}

void zmq_server::out_handler(std::stop_token st) { //SMSC -> NUMS -> ZMQ
    while (!st.stop_requested()) {
    
    }
}

