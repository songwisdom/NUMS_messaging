#include "configuration.hpp"
#include <iostream>

#include "loglevel_checker.hpp"
#include "spdlog/async.h"
#include "spdlog/fmt/bin_to_hex.h"
#include "spdlog/sinks/daily_file_sink.h"
#include "spdlog/spdlog.h"
#include "spdlog_custom_daily_filename_calculator.h"
#include "spdlog_custom_hourly_filename_calculator.h"

#include <boost/asio.hpp>
#include <boost/asio/awaitable.hpp>
#include <boost/asio/co_spawn.hpp>
#include <boost/asio/detached.hpp>
#include <boost/asio/use_awaitable.hpp>

#include <array>
#include <iostream>
namespace net = boost::asio;
using tcp = net::ip::tcp;
using net::awaitable;
using net::use_awaitable;

#define worker_log_pattern "%m-%d %T.%e [%L] %v"

struct TpsDelay {
  int min;
  int max;
  int delay;
  int file_size;
};

void static_config_init();
void local_config_init();
void log_init();
bool init();

static std::unique_ptr<loglevel_checker> g_llc;
static std::jthread g_thr_llc;

static bool is_normal_disconnect(const boost::system::error_code &ec) {
  return ec == net::error::eof || ec == net::error::connection_reset ||
         ec == net::error::operation_aborted;
}

awaitable<void> session(tcp::socket socket) {
  std::array<char, 4096> data{};

  try {
    for (;;) {
      std::size_t n =
          co_await socket.async_read_some(net::buffer(data), use_awaitable);
      co_await net::async_write(socket, net::buffer(data.data(), n),
                                use_awaitable);
    }
  } catch (const boost::system::system_error &e) {
    if (!is_normal_disconnect(e.code())) {
      std::cerr << "[session] " << e.code().message() << "\n";
    }
  } catch (const std::exception &e) {
    std::cerr << "[session] " << e.what() << "\n";
  }

  co_return;
}

awaitable<void> listener(unsigned short port) {
  auto ex = co_await net::this_coro::executor;

  tcp::acceptor acceptor(ex);
  acceptor.open(tcp::v4());
  acceptor.set_option(net::socket_base::reuse_address(true));
  acceptor.bind({tcp::v4(), port});
  acceptor.listen();

  std::cerr << "listening on 0.0.0.0:" << port << "\n";

  for (;;) {
    tcp::socket socket = co_await acceptor.async_accept(use_awaitable);
    net::co_spawn(ex, session(std::move(socket)), net::detached);
  }
}

int main(int argc, char **argv) {
  init();

  unsigned short port = 9000;
  if (argc >= 2)
    port = static_cast<unsigned short>(std::stoi(argv[1]));

  try {
    net::io_context ctx;

    net::co_spawn(ctx, listener(port), net::detached);

    ctx.run();
  } catch (const std::exception &e) {
    std::cerr << "fatal: " << e.what() << "\n";
    return 1;
  }

  while (1) {
    spdlog::info("info log");
    spdlog::debug("debug log");
    std::this_thread::sleep_for(std::chrono::seconds(1));
  }

  return 0;
}

bool init() {
  // 전역 설정 초기화
  if (!configuration::init("/svc/simu/config/simu.toml")) {
    std::cerr << "failed to load simu.toml\n";
    return false;
  }

  const std::string log_dir =
      configuration::get_string("common", "log_dir", "/logs/simu");
  const std::string log_fpath =
      fmt::format("{}/{}/{}", log_dir, "frame", "FRAME");
  spdlog::set_default_async_q_size(8192);
  auto logger = spdlog::create_async_dn<spdlog::sinks::hourly_file_sink<
      std::mutex, custom_hourly_filename_calculator>>("worker", log_fpath,
                                                      false);
  logger->set_pattern(worker_log_pattern);
  spdlog::set_default_logger(logger);
  spdlog::set_level(spdlog::level::info);
  spdlog::flush_on(spdlog::level::info);
  spdlog::flush_every(std::chrono::seconds(3));

  spdlog::info("hello world");

  g_llc = std::make_unique<loglevel_checker>(
      "frame", configuration::get_string("frame", "log_level", "info"));

  g_thr_llc = std::jthread([&](std::stop_token st) { g_llc->start(st); });

  return true;
}

void log_init() {
  const std::string log_dir =
      configuration::get_string("common", "log_dir", "/logs/simu");
  const std::string log_fpath =
      fmt::format("{}/{}/{}", log_dir, "frame", "FRAME");
  spdlog::set_default_async_q_size(8192);
  auto logger = spdlog::create_async_dn<spdlog::sinks::hourly_file_sink<
      std::mutex, custom_hourly_filename_calculator>>("worker", log_fpath,
                                                      false);
  logger->set_pattern(worker_log_pattern);
  spdlog::set_default_logger(logger);
  spdlog::set_level(spdlog::level::info);
  spdlog::flush_on(spdlog::level::info);
  spdlog::flush_every(std::chrono::seconds(3));
}

void static_config_init() {
  // 전역 설정 초기화
  if (!configuration::init("/svc/simu/config/simu.toml")) {
    std::cerr << "failed to load simu.toml\n";
    return;
  }

  // server 설정 읽기
  std::string server_ip =
      configuration::get_string("server", "ip", "0.0.0.0"); // default 다름
  int server_port =
      configuration::get_int("server", "port", 9999); // default 다름
  bool server_debug =
      configuration::get_bool("server", "debug", false); // default 다름

  // database.master 설정 읽기
  std::string db_host =
      configuration::get_string("database.master", "host", "localhost");
  std::string db_user =
      configuration::get_string("database.master", "user", "root");
  const std::vector<int> db_ports =
      configuration::get_array_int("database.master", "ports", {1111, 2222});

  std::cout << fmt::format("[server]\n"
                           "  ip    = {}\n"
                           "  port  = {}\n"
                           "  debug = {}\n",
                           server_ip, server_port,
                           server_debug ? "true" : "false");

  std::cout << fmt::format("[database.master]\n"
                           "  host  = {}\n"
                           "  user  = {}\n",
                           db_host, db_user);

  std::cout << "  ports = ";
  for (size_t i = 0; i < db_ports.size(); ++i) {
    std::cout << db_ports[i];
    if (i + 1 < db_ports.size())
      std::cout << ", ";
  }
  std::cout << "\n";

  std::string ip = configuration::get_string("sigmo.server", "ip", "127.0.0.1");
  std::cout << "simmo.server" << ip << std::endl;

  auto tables = configuration::get_table_array("sigmo", "tps_delay");

  std::vector<TpsDelay> delays;
  for (const auto &tbl : tables) {
    TpsDelay d;
    d.min = tbl["min"].value_or(0);
    d.max = tbl["max"].value_or(0);
    d.delay = tbl["delay"].value_or(0);
    d.file_size = tbl["file_size"].value_or(0);
    delays.push_back(d);
  }

  for (const auto &d : delays) {
    std::cout << "min=" << d.min << " max=" << d.max << " delay=" << d.delay
              << " file_size=" << d.file_size << "\n";
  }
}

void local_config_init() {
  // Configuration 로컬 인스턴스
  configuration::Configuration cfg;

  if (!cfg.init("/svc/simu/config/simu.toml")) {
    std::cerr << "failed to load simu.toml\n";
    return;
  }

  // server 설정
  std::string server_ip = cfg.get_string("server", "ip", "0.0.0.0");
  int server_port = cfg.get_int("server", "port", 9999);
  bool server_debug = cfg.get_bool("server", "debug", false);

  // database.master 설정
  std::string db_host = cfg.get_string("database.master", "host", "localhost");
  std::string db_user = cfg.get_string("database.master", "user", "root");

  std::vector<int> db_ports =
      cfg.get_array_int("database.master", "ports", {1111, 2222});

  // 출력
  std::cout << fmt::format("[server]\n"
                           "  ip    = {}\n"
                           "  port  = {}\n"
                           "  debug = {}\n",
                           server_ip, server_port,
                           server_debug ? "true" : "false");

  std::cout << fmt::format("[database.master]\n"
                           "  host  = {}\n"
                           "  user  = {}\n",
                           db_host, db_user);

  std::cout << "  ports = ";
  for (size_t i = 0; i < db_ports.size(); ++i) {
    std::cout << db_ports[i];
    if (i + 1 < db_ports.size())
      std::cout << ", ";
  }
  std::cout << "\n";
}
