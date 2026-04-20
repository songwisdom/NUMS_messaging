#pragma once

#include "common.hpp"
#include "zmq_monitor.hpp"

#include <atomic>
#include <thread>
#include <zmq.hpp>

class zmq_client
{
public:
    zmq_client();
    ~zmq_client();

    void start(int idx);

    void stop();
    void input_msg(nums_msg &msg, rapidjson::StringBuffer &s);

private:
    std::atomic<bool> stop_{false};

    zmq::context_t ctx_{1};
    zmq::socket_t  sock_{ctx_, zmq::socket_type::dealer}; //req -> dealer

    plk_monitor_t monitor_;
    std::thread   mon_th_;
};