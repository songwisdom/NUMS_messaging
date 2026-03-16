#pragma once
#include "common.hpp" //log위함
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstddef>
#include <cerrno>
#include <arpa/inet.h>
#include <array>
#include <iostream>
#include <cstdint>

class TcpClient
{
    public:
        TcpClient() = default;
        bool connect(const std::string& ip, uint16_t port);
        bool send_all(const std::byte* data, std::size_t len);
        bool read_exact(std::byte* buf, size_t len);
        void close();
        // bool is_connected() const { return fd_ >= 0; }

    private:
        int fd_{-1};
};