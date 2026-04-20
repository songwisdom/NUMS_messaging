#pragma once
#include "common.h"
#include "zmq_monitor.hpp"
#include "dataPacket.hpp"
#include "ThreadSafeQueue.hpp"
#include <atomic>
#include <thread>
#include <zmq.hpp>
#include <unordered_map>

class zmq_server
{
    public:
        explicit zmq_server(ThreadSafeQueue<nums::Packet>& outq,
                        ThreadSafeQueue<nums::Packet>& inq);
        ~zmq_server();
        void start();
        void stop();
    private:
        ThreadSafeQueue<nums::Packet> queue_;
        ThreadSafeQueue<nums::Packet>& outq_; // NUMS -> SMSC 
        ThreadSafeQueue<nums::Packet>& inq_; // ZMQ_CLI <- NUMS

        std::jthread in_handler_th_;
        std::jthread out_handler_th_;
        void in_handler(std::stop_token st); //zmq dealer -> NUMS
        void out_handler(std::stop_token st); //NUMS 처리 결과

        std::atomic<bool> stop_{false};
        zmq::context_t ctx_{1};
        zmq::socket_t  sock_{ctx_, zmq::socket_type::router}; //rep -> router
        plk_monitor_t monitor_;
        std::jthread mon_th_;
        zmq::pollitem_t items[];
};