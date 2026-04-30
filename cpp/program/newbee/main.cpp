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
//#include "zmq.hpp"
#include "zmq_server.hpp"
#include "ThreadSafeQueue.hpp"
#include "dataPacket.hpp"
#include "numsworker.hpp"
namespace net = boost::asio;

int main() {
    logging::log_init("NUMS");
    net::io_context io;

    ThreadSafeQueue<nums::Packet> outq_;
    ThreadSafeQueue<nums::Packet> inq_;

    zmq_server svr(outq_, inq_);
    numsworker wkr(outq_, inq_);

    svr.start();
    wkr.start(); 

    net::steady_timer t(io, std::chrono::seconds(20));
    t.async_wait([&](const boost::system::error_code& ec) {
        if (!ec) {
            std::cout << "Time is up. Stopping servers..." << std::endl;
            svr.stop();
            wkr.stop();
            s.cancel();
            io.stop();
        }
    });

    svr.stop();
    wkr.stop();

    io.run();
    return 0;
}


/*
std::jthread t_zsvr([&svr]() { // ZMQ_SERVER
    svr.start();
});
*/

/*
    (선택사항)
    객체를 스마트포인터로 생성하는 방식으로 수명 관리
    auto svr = std::make_shared<zmq_server>();
    std::thread t_zsvr([svr]{
        svr->start();
    });
*/

//zmq_rcver, smsc_rcver 두개 -> epoll?
/*
    std::jthread th_timer(timer);
    std::jthread th_zmqrcver(zmqrcver);
    
    std::this_thread::sleep_for(std::chrono::milliseconds(15000)); 
    // 중단 요청, jt 소멸 시 자동으로 조인
    th_timer.request_stop();
    th_zmqrcver.request_stop();
*/

/*
//비동기
//inq_ 모니터링 -> 무조건 다 smsc 로 보냄. 5가 왜있는지 모르겠음 zmq_cli 에
//일괄 smsc로 전송(type : 1,3,6)
net::awaitable<void> inq_observer(){
    auto exec = co_await net::this_coro::executor;
    net::steady_timer t(exec);
    while(true){
        t.expires_after(std::chrono::seconds(2));
        co_await t.async_wait(net::use_awaitable);
        if(!inq_.empty()){
            auto x = inq_.pop();
            retry_map.push(x); // try_cnt, seq_num, send_time
        }
    }
}

//비동기
net::awaitable<void> zmq_rcver(){ // epoll(멀티플렉싱 - 외부 client N개)
// (zmq server bind)
    
// 메시지(JSON, msg type(3 or ?)) JSON 들어오면
// JSON->객체->패킷 후 inq_에 put
//connect 한 곳에서 데이터 들어오면 JSON->패킷화해서 전역 inq_에 put
//동시에 zmq_req_processor 호출

}

//비동기
void smsc_rcver(){
    //smsc로부터 패킷 받음 (type 2,4,5)
    //패킷->객체 후 seq_num이 retrymap에 있다면 retrymap.remove(seq_num) - 보통 2,4 type
    //switch -> type 5라면 JSON화 시킨 후 zmq_client 에게 전송, type 6을 만들어서 전역 inq_에 put
}

//비동기
void retrymap_observer(){
    //retrymap (unordered map)
    //retrymap.send_time 이 10초 이전&&try_cnt=1 인 데이터를 smsc로 재전송
    //retrymap.send_time 이 10초 이전&&try_cnt=2 -> connection 해제 후 재수립
}

//비동기
net::awaitable<void> health_timer(net::io_context& io){
    auto exec = co_await net::this_coro::executor;
    net::steady_timer t(exec);
    while(true){
        t.expires_after(std::chrono::seconds(3));
        co_await t.async_wait(net::use_awaitable); //expire 걸어놓고 기다리게 함
        if(!runsc::health_chker()){
            std::cout << "fatal error → stopping program\n";
            io.stop(); //이벤트루프 종료
            co_return;
        }
    }
}

net::co_spawn(io, health_timer(io), net::detached);

*/


//사용된 queue.pop(),read_exact 전부 블로킹이라 메시지 없으면 멈춰있음
//health chker 스레드 필요