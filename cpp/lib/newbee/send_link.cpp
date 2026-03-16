#include <array>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstddef>
#include <cerrno>
#include <arpa/inet.h>
#include "spdlog/async.h"
#include "spdlog/fmt/bin_to_hex.h"
#include "spdlog/sinks/daily_file_sink.h"
#include "spdlog/spdlog.h"
#include "spdlog_custom_daily_filename_calculator.h"
#include "spdlog_custom_hourly_filename_calculator.h"
#include "configuration.hpp"
#include "dataElement.hpp"
#include "dataPacket.hpp"
#include "nums_constants.hpp"

#define worker_log_pattern "%m-%d %T.%e [%L] %v"

inline bool read_exact(int fd, std::byte* buffer, std::size_t total) {
    std::size_t received = 0;
    while (received < total) {
        ssize_t n = ::recv(fd, buffer + received, total - received, 0);
        if (n == 0) { // 상대의 정상 연결 종료
            return false;
        }
        if (n < 0) {
            if (errno == EINTR) { // 시그널에 의해 중단 → 다시 시도
                continue;
            }
            return false;
        }
        received += static_cast<std::size_t>(n);
    }
    return true;
}

namespace logging{
    void log_init(std::string logtype) {
        const std::string log_dir =
            configuration::get_string("common", "log_dir", "/logs/nums/");
        const std::string log_fpath =
            fmt::format("{}/{}/{}", log_dir, logtype, logtype); // "디렉토리" "log헤더"
        spdlog::set_default_async_q_size(8192);
        auto logger = spdlog::create_async_dn<spdlog::sinks::hourly_file_sink<
            std::mutex, custom_hourly_filename_calculator>>("worker", log_fpath,false);
        logger->set_pattern(worker_log_pattern);
        spdlog::set_default_logger(logger);
        spdlog::set_level(spdlog::level::info);
        spdlog::flush_on(spdlog::level::info);
        spdlog::flush_every(std::chrono::seconds(3));
    }
}

namespace runsc {
    bool health_chker() { //5초 주기 health check
        spdlog::info("[Health CHK] health_chker init");
        int fd = ::socket(AF_INET, SOCK_STREAM, 0);

        sockaddr_in addr{};
        addr.sin_family = AF_INET;
        addr.sin_port = htons(8500);
        inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);

        int ret = ::connect(fd, (sockaddr*)&addr, sizeof(addr));
        if(ret < 0){
            spdlog::error(
                "connect failed errno={} ({})",
                errno, strerror(errno)
            );
            spdlog::error("[Health CHK] Connect Failed. try again");
            ::close(fd);
            return false;
        }
        spdlog::info("[Health CHK] Connected.");

        nums::Packet link_p;
        nums::Header h;
        h.set_msg_type(MsgType::LINK);
        link_p.header = h;
        
        std::array<std::byte, nums::Header::SIZE> out{};
        link_p.serialize(out.data(), out.size()); // packet serialize
        ::send(fd, out.data(), out.size(), 0);

        spdlog::info("[Health CHK] sent:{}\n", link_p.toString());
        std::array<std::byte, nums::Header::SIZE> in_buf{}; // 수신
        if (!read_exact(fd, in_buf.data(), in_buf.size())) {
            spdlog::info("[Health CHK] read failed\n");
            return false;
        }
        nums::Header ack;
        ack.deserialize(in_buf.data());
        spdlog::info("[Health CHK] received:{}\n",ack.toString());

        ::close(fd);
        return true;
    }
}



/*
    void link_receiver(){
        ///////////////////////////////////////////////////
        int listen_fd = ::socket(AF_INET, SOCK_STREAM, 0);

        sockaddr_in addr{};
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = htonl(INADDR_ANY);
        addr.sin_port = htons(9000);

        ::bind(listen_fd, (sockaddr*)&addr, sizeof(addr));
        ::listen(listen_fd, 1);

        logging::log_init("server");
        spdlog::info("[server] listening...");
        int client_fd = ::accept(listen_fd, nullptr, nullptr);
        spdlog::info("[server] client connected");
        ///////////////////////////////////////////////////

        // 1) 헤더 수신
        std::array<std::byte, nums::Header::SIZE> buf{};
        if (!read_exact(client_fd, buf.data(), buf.size())) {
            spdlog::info("[server] read failed\n");
            return;
        }

        nums::Header h;
        h.deserialize(buf.data());
        spdlog::info("[server] received:{}", h.toString());

        // 2) 즉시 ACK 송신
        nums::Header ack;
        ack.set_msg_type(MsgType::LINK_ACK);
        ack.set_msg_len(nums::type_to_msglen(MsgType::LINK_ACK));
        std::array<std::byte, nums::Header::SIZE> out_buf{};
        ack.serialize(out_buf.data());
        ::send(client_fd, out_buf.data(), out_buf.size(), 0);
        spdlog::info("[server] sent:{}\n", ack.toString());

        ::close(client_fd);
        ::close(listen_fd);
    }
*/