#pragma once
#include "nums_constants.hpp"
#include <array>
#include <sys/socket.h>
#include "dataElement.hpp"
#include "dataPacket.hpp"

inline bool send_link(int fd,int msg_type){
    nums::Header h;
    std::array<std::byte, nums::Header::SIZE> buf{};
    h.serialize(buf.data());

    ssize_t n = ::send(fd, buf.data(), buf.size(), 0);
    return n == static_cast<ssize_t>(buf.size());
}

namespace runsc {
    bool health_chker();
    // void connect_chker();
}

namespace logging{
    void log_init(std::string logtype);
}
