#include <iostream>
#include <array>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>
// Boost
#include <boost/asio.hpp>
#include <boost/asio/awaitable.hpp>
#include <boost/asio/co_spawn.hpp>
#include <boost/asio/detached.hpp>
#include <boost/asio/use_awaitable.hpp>

// custom
#include "configuration.hpp"
#include "send_link.hpp"
#include "loglevel_checker.hpp"
#include <stop_token>

#include "common.h"
#include "zmq_server.hpp"
#include "ThreadSafeQueue.hpp"
#include "dataPacket.hpp"
#include "numsworker.hpp"
namespace net = boost::asio;

int main() {
    logging::log_init("NUMS");
    net::io_context io_;

    ThreadSafeQueue<nums::Packet> inq_;
    
    numsworker wkr(io_, retry_map, inq_);
    zmq_server svr(io_, inq_,
        [&](nums::Packet p) { 
            wkr.handle_request(std::move(p)); 
        }
    );
    
    wkr.start(); //NUMS
    svr.start(); //ZMQ Server

    net::steady_timer t(io_, std::chrono::seconds(20));
    t.async_wait([&](const boost::system::error_code& ec) { //코루틴??
        if (!ec) {
            std::cout << "Time is up. Stopping servers..." << std::endl;
            wkr.stop();
            svr.stop();
            s.cancel();
            io_.stop();
        }
    });

    net::signal_set s(io_, SIGINT, SIGTERM);
    s.async_wait([&](const boost::system::error_code& ec, int) {
        if (!ec) {
            std::cout << "Interrupt received. Stopping servers..." << std::endl;
            t.cancel();
            wkr.stop();
            svr.stop();
            io_.stop();
        }
    });

    io_.run();
    return 0;
}



// 콜백 - 객체참조캡처 vs shared_ptr vs static
// wkr 생성 없이 wkr::handle_request()로 바로 호출 혹은 static 함수로 바꾸면 가능하긴 함. 
// 하지만, 상태변수 공유 어렵고, 멤버 함수로 두는 게 객체 지향적 설계에 더 맞음.
// 지금처럼 참조 캡처 유지




// poll로 event 감지 후 json parsing, Packet화 해서 io.post
// inq_ 모니터링-zmq client 에 응답
// thread 수 : 2개 (mon_th_, in_handler_th_)

/*
zmq_server svr(io_, inq_,
    [&](nums::Packet p) { 
        wkr.handle_request(std::move(p)); // 참조캡처
    }
);
->수명 주의(람다 실행 시 wkr가 소멸됐다면 dangling reference)->std::shared_ptr로 해결 가능하지만,
main 함수에서 wkr과 svr의 수명은 프로그램 전체이므로 dangling reference는 발생하지 않음
이런 경우 shared_ptr로 감싸는 것은 오히려 불필요한 참조 카운트 관리 비용 발생시킴(오버헤드),
설계가 느슨해짐, 의존성이 숨겨질 수 있음(“왜 이 객체가 여기서도 살아있지?”)

auto wkr = std::make_shared<numsworker>(io_, retry_map, inq_);
zmq_server svr(io_, inq_,
    [wkr](nums::Packet p) {
        wkr->handle_request(std::move(p));
    }
);

결론 : wkr가 svr보다 확실히 오래 산다: 지금처럼 참조 캡처 유지
콜백이 더 오래 살아서 수명이 불명확하다: shared_ptr 고려
단, shared_ptr를 쓰면 wkr를 캡처하는 람다도 wkr->handle_request(...) 형태로 바꿔야 함


*/