#include "configuration.hpp"
#include "zmq_client.hpp"
#define worker_log_pattern "%m-%d %T.%e [%L] %v"

static std::unique_ptr<loglevel_checker> g_llc;
static std::jthread g_thr_llc;

bool init(int argc, char **argv) {
  // 전역 설정 초기화
  if (!configuration::init("/svc/simu/config/simu.toml")) {
    std::cerr << "failed to load simu.toml\n";
    return false;
  }

  const std::string log_dir =
      configuration::get_string("common", "log_dir", "/logs/simu");
  const std::string log_fpath =
      fmt::format("{}/{}/{}", log_dir, "zmqsvr", "ZMQSVR");
  spdlog::set_default_async_q_size(8192);
  auto logger = spdlog::create_async_dn<spdlog::sinks::hourly_file_sink<
      std::mutex, custom_hourly_filename_calculator>>("worker", log_fpath,
                                                      false);
  logger->set_pattern(worker_log_pattern);
  spdlog::set_default_logger(logger);
  spdlog::set_level(spdlog::level::info);
  spdlog::flush_on(spdlog::level::info);
  spdlog::flush_every(std::chrono::seconds(3));

  spdlog::info("zmq client.");

  g_llc = std::make_unique<loglevel_checker>(
      "zmqsvr", configuration::get_string("zmqsvr", "log_level", "info"));

  g_thr_llc = std::jthread([&](std::stop_token st) { g_llc->start(st); });

  return true;
}

int main(int argc, char **argv) {
    init(argc, argv);
    constexpr int CLIENT_COUNT = 1;

    std::vector<std::unique_ptr<zmq_client>> clients;
    std::vector<std::thread> threads;

    clients.reserve(CLIENT_COUNT);
    threads.reserve(CLIENT_COUNT);

    for (int i = 0; i < CLIENT_COUNT; ++i) {
        clients.emplace_back(std::make_unique<zmq_client>());

        threads.emplace_back([client = clients.back().get(), i]() {
            spdlog::info("client thread {} start", i);
            client->start(i);
        });
    }

  while (1) {
    spdlog::debug("[Zclient]debug log");
    std::this_thread::sleep_for(std::chrono::seconds(1));
  }

  return 0;
}