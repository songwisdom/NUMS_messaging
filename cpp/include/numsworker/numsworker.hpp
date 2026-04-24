#pragma once
#include "common.hpp"
#include "ThreadSafeQueue.hpp"
#include "dataPacket.hpp"
#include "dataElement.hpp"
#include "nums_constants.hpp"
#include <atomic>
#include <thread>
#include "TcpClient.hpp"
#include <string>
#include <cstdint>
#include <array>

class numsworker
{
    public:
        explicit numsworker(ThreadSafeQueue<nums::Packet>& outq,
                        ThreadSafeQueue<nums::Packet>& inq);
        ~numsworker();
        void start();
        void stop();

    private:
        std::jthread outq_thread_;
        ThreadSafeQueue<nums::Packet>& outq_; // NUMS -> SMSC
        ThreadSafeQueue<nums::Packet>& inq_; // NUMS -> ZMQ Client
        void outq_monitor(std::stop_token st); // smsc로 req
        void inq_monitor(std::stop_token st); // zmq client로 res
        
        bool sendMsg(const nums::Packet& msg);
        std::optional<nums::Packet> recvMsg();
        std::optional<nums::Header> recvHeader();
        std::optional<nums::Body> recvBody(nums::Header h);
        bool sendMsg(std::optional<nums::Packet> msg);
        bool send_all(int fd, const std::byte* data, size_t len);

        TcpClient smsc_; 
        std::string serv_host = "112.172.129.233"; //"127.0.0.1"
        uint16_t serv_port = 8500;
};



// void healthChkstart();
// std::jthread health_checker_thread_;
// void health_chker(std::stop_token st); // Health Checker
// std::atomic<bool> connected_{false};
// std::atomic<bool> reconnect_requested_{false};
// std::atomic<bool> idle_mode{true}; //메시지 있으면 false
// using clock = std::chrono::steady_clock;
// std::atomic<clock::time_point> last_ok_{clock::now()};