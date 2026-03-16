#include "server.hpp"
#include "coroutine.h"

#include <boost/asio.hpp>
#include <boost/asio/awaitable.hpp>
#include <boost/asio/co_spawn.hpp>
#include <boost/asio/detached.hpp>
#include <boost/asio/signal_set.hpp>
#include <boost/asio/use_awaitable.hpp>

#include <array>
#include <iostream>
#include <thread>
#include <vector>

namespace net = boost::asio;
using tcp = net::ip::tcp;
using net::awaitable;
using net::use_awaitable;

static bool is_normal_disconnect(const boost::system::error_code& ec) {
    return ec == net::error::eof ||
           ec == net::error::connection_reset ||
           ec == net::error::operation_aborted;
}

struct Server::Impl {
    net::io_context ctx;
    net::executor_work_guard<net::io_context::executor_type> work;
    std::vector<std::thread> workers;

    std::uint16_t port;
    int threads;

    Impl(std::uint16_t p, int t)
        : ctx()
        , work(net::make_work_guard(ctx))
        , port(p)
        , threads(t < 1 ? 1 : t)
    {}

    static awaitable<void> session(tcp::socket socket) {
        std::array<char, 4096> data{};

        try {
            for (;;) {
                std::size_t n =
                    co_await socket.async_read_some(net::buffer(data), use_awaitable);

                if (n == 0) break;

                co_await net::async_write(
                    socket, net::buffer(data.data(), n), use_awaitable);
            }
        } catch (const boost::system::system_error& e) {
            if (!is_normal_disconnect(e.code())) {
                std::cerr << "[session] " << e.code().message() << "\n";
            }
        }
        co_return;
    }

    static awaitable<void> listener(std::uint16_t port) {
        auto ex = co_await net::this_coro::executor;

        tcp::acceptor acceptor(ex);
        acceptor.open(tcp::v4());
        acceptor.set_option(net::socket_base::reuse_address(true));
        acceptor.bind({tcp::v4(), port});
        acceptor.listen(net::socket_base::max_listen_connections);

        std::cerr << "[server] listening on 0.0.0.0:" << port << "\n";

        for (;;) {
            tcp::socket socket =
                co_await acceptor.async_accept(use_awaitable);
            net::co_spawn(ex, session(std::move(socket)), net::detached);
        }
    }

    void start() {
        net::co_spawn(ctx, listener(port), net::detached);

        for (int i = 0; i < threads; ++i) {
            workers.emplace_back([this] {
                ctx.run();
            });
        }
    }

    void stop() {
        ctx.stop();
        for (auto& t : workers) {
            if (t.joinable())
                t.join();
        }
        workers.clear();
    }
};

Server::Server(std::uint16_t port, int threads)
    : impl_(std::make_unique<Impl>(port, threads))
{}

Server::~Server() {
    stop();
}

void Server::start() {
    impl_->start();
}

void Server::stop() {
    if (impl_)
        impl_->stop();
}
