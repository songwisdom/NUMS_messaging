#pragma once
#include "common.hpp"
#include "ThreadSafeQueue.hpp"
#include "dataPacket.hpp"
#include "dataElement.hpp"
#include "nums_constants.hpp"
#include <atomic>
#include <thread>
#include "TcpClient.hpp"

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

        bool send_all(int fd, const std::byte* data, size_t len);

        TcpClient smsc_; // ⭐
};