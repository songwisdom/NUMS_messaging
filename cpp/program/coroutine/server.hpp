#pragma once

#include <cstdint>
#include <memory>

class Server {
public:
    Server(std::uint16_t port, int threads = 1);
    ~Server();

    void start();   // 서버 기동 (비동기)
    void stop();    // 서버 종료

private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
};
