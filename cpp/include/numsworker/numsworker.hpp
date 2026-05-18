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
//Boost
#include <boost/asio/io_context.hpp>

class numsworker
{
    public:
        explicit numsworker(net::io_context& io,
                    ThreadSafeQueue<nums::Packet>& outq,
                    ThreadSafeQueue<nums::Packet>& inq);
        ~numsworker();
        void start();
        void stop();
        void handle_request(nums::Packet p); //또는 nums::Packet&& (main의 io_context 가 실행할 함수)

    private:
        net::io_context& io_;
        std::jthread outq_thread_;
        std::atomic<bool> connected_{false};

        ThreadSafeQueue<nums::Packet>& inq_; // NUMS -> ZMQ Client
        std::unordered_map<std::string, RetryInfo> retry_map_;
        void inq_monitor(std::stop_token st); // zmq client로 res

        std::optional<nums::Header> recvHeader();
        std::optional<nums::Packet> recvMsg();
        std::optional<nums::Body> recvBody(nums::Header h);

        bool sendMsg(const nums::Packet& msg);
        bool send_all(int fd, const std::byte* data, size_t len);
        void async_send(const nums::Packet& msg);
            
        TcpClient smsc_; 
        std::string serv_host = "112.172.129.233"; //"127.0.0.1"
        uint16_t serv_port = 8500;
        int retry_cnt = 0;

};

