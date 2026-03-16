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

bool init(int argc, char **argv);
