#pragma once
#include "common.h"
#include "zmq_monitor.hpp"
#include "dataPacket.hpp"
#include "ThreadSafeQueue.hpp"
#include <atomic>
#include <thread>
#include <zmq.hpp>
#include <unordered_map>
// Boost
#include <boost/asio.hpp>

namespace net = boost::asio;
// using RequestHandler = std::function<void(nums::Packet)>; //대충 이렇게 alias 쓴다

class zmq_server
{
    public:
        explicit zmq_server(
                net::io_context& io,
                ThreadSafeQueue<nums::Packet>& inq,
                std::function<void(nums::Packet)> req_handler
            );
        ~zmq_server();
        void start();
        void stop();
    private:
        net::io_context& io_;
        ThreadSafeQueue<nums::Packet>& inq_; // ZMQ_CLI <- NUMS
        
        std::function<void(nums::Packet)> req_handler_; 
        
        std::jthread in_handler_th_;
        void in_handler(std::stop_token st); //zmq dealer -> NUMS

        std::atomic<bool> stop_{false};
        zmq::context_t ctx_{1};
        zmq::socket_t  sock_{ctx_, zmq::socket_type::router}; //rep -> router
        plk_monitor_t monitor_;
        std::jthread mon_th_;
        zmq::pollitem_t items[];
};