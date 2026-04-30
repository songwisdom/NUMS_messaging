#include "TcpClient.hpp"
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstddef>
#include <cerrno>
#include <arpa/inet.h>
#include <array>
#include <iostream>
#include <cstdint>

bool TcpClient::connect(const std::string& ip, uint16_t port)
{
    fd_ = ::socket(AF_INET, SOCK_STREAM, 0);
    if (fd_ < 0)
        return false;
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    inet_pton(AF_INET, ip.c_str(), &addr.sin_addr);

    int ret = ::connect(fd_, (sockaddr*)&addr, sizeof(addr));
    if (ret < 0){
        ::close(fd_);
        fd_ = -1;
        return false;
    }
    // timeout 설정
    timeval tv{};
    // tv.tv_sec = 5; 
    tv.tv_usec = 0;
    // 수신 타임아웃
    setsockopt(fd_, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
    return true;
}


bool TcpClient::send_all(const std::byte* data, std::size_t len)
{
    std::size_t sent = 0;
    while (sent < len)
    {
        ssize_t n = ::send(fd_, data + sent, len - sent, 0);
        if (n <= 0)
            return false;
        sent += n;
    }
    return true;
}


bool TcpClient::read_exact(std::byte* buf, size_t len)
{
    size_t received = 0;
    while (received < len){
        ssize_t n = ::recv(fd_, buf + received, len - received, 0);
        if (n > 0) {
            received += static_cast<size_t>(n);
            continue;
        }
        if (n == 0) {
            spdlog::error("peer closed connection");
            return false;
        }
        if (errno == EINTR)
            continue;
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            spdlog::warn("recv timeout");
            return false;
        }
        spdlog::error("recv failed errno={} ({})", errno, strerror(errno));
        return false;
    }
    return true;
}

void TcpClient::close(){
    if(fd_ != -1){
        ::close(fd_);
        fd_ = -1;
    }
    return;
}