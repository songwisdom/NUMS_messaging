#include <iostream>
#include "Worker.hpp"
#include "configuration.hpp"

#include <thread>

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

static std::jthread t;

void static_config_init();
bool init();

int main(int argc, char **argv){
  // 전역 설정 초기화
    Worker w;
    t(&Worker::work, &w);
    spdlog::info("Main thread: doing something else");
    t.join();
    
}




