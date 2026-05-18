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
// Boost
#include <boost/asio.hpp>

namespace net = boost::asio;

zmq_server::zmq_server(
            net::io_context& io,
            ThreadSafeQueue<nums::Packet>& inq,
            std::function<void(nums::Packet)> req_handler
            ): 
        io_(io),
        inq_(inq),
        req_handler_(std::move(req_handler))
{}

zmq_server::~zmq_server()
{ stop(); }

void zmq_server::start()
{
    spdlog::info("ZMQ Server up.");
    sock_.set(zmq::sockopt::linger, 0);
    sock_.set(zmq::sockopt::rcvhwm, 30000);
    sock_.set(zmq::sockopt::rcvtimeo, 0); //0 맞나 확인하셈

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
}

void zmq_server::stop()
{ //소멸 순서
    in_handler_th_.request_stop();
    mon_th_.request_stop();
}

void zmq_server::in_handler(std::stop_token st) {
    // ZMQ 소켓 poll -> event 감지 -> io_context 에 post(데이터, 실행할 함수)
    while (!st.stop_requested()) {
        zmq::pollitem_t items[] = {
            { static_cast<void*>(sock_), 0, ZMQ_POLLIN, 0 }
        };
        zmq::poll(items, 1, 100ms); // (socket 1개, 100ms wake&loop)
        if (items[0].revents & ZMQ_POLLIN) {
            std::vector<zmq::message_t> recv_frames;
            do {
                zmq::message_t msg;
                sock_.recv(msg, zmq::recv_flags::none); 
                // recv (none옵션: blocking, dontwait: non-blocking-busy)
                // start()함수에서 sockopt 확인해라...
                recv_frames.emplace_back(std::move(msg)); //이동 생성자를 타게 함
            } while (sock_.get(zmq::sockopt::rcvmore));

            if (recv_frames.size() < 2) {
                spdlog::warn("invalid ROUTER message: frame_count={}", recv_frames.size());
            } else {
                auto& payload_msg = recv_frames.back(); //마지막 원소 참조
                auto p = nums::Packet::parse_json(
                    static_cast<const std::byte*>(payload_msg.data()),
                    payload_msg.size()
                );
                p.set_identity(recv_frames[0]); //std::move (모르고 갈겨둔 move)
                net::post(io_, [this, p = std::move(p)]() mutable { //io_context에 콜백 등록
                    req_handler_(std::move(p)); 
                });
            }
        }
        //ROUTER
        while(auto reply = inq_.try_pop(st)){ //try_pop (없으면 계속 null 반환) > notify 기반 blocking pop(O)
            //다음 세 줄을 별도 스레드로 분리 - 하려면 inq_.push() 하는 부분 (SMSC receiver)에서 수행해줘야함, type도 바꿔야함 
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