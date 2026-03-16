#pragma once
#include "configuration.hpp"
#include <iostream>
#include "loglevel_checker.hpp"
#include "spdlog/async.h"
#include "spdlog/fmt/bin_to_hex.h"
#include "spdlog/sinks/daily_file_sink.h"
#include "spdlog/spdlog.h"
#include "spdlog_custom_daily_filename_calculator.h"
#include "spdlog_custom_hourly_filename_calculator.h"

bool init(int argc, char **argv);