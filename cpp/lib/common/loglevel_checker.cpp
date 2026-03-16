#include <boost/algorithm/string.hpp>
#include "loglevel_checker.hpp"
#include "configuration.hpp"
#include "spdlog/spdlog.h"
#include <thread>
#include <chrono>
#include <stop_token>

static spdlog::level::level_enum to_spdlog_level(const std::string& ll) {
    if (boost::iequals(ll, "off"))      return spdlog::level::off;
    if (boost::iequals(ll, "critical")) return spdlog::level::critical;
    if (boost::iequals(ll, "error"))    return spdlog::level::err;
    if (boost::iequals(ll, "warn"))     return spdlog::level::warn;
    if (boost::iequals(ll, "debug"))    return spdlog::level::debug;
    if (boost::iequals(ll, "trace"))    return spdlog::level::trace;
    return spdlog::level::info;
}

void loglevel_checker::start(std::stop_token st) {
    // 초기 레벨 적용(원하면)
    spdlog::set_level(to_spdlog_level(log_level_));

    while (!st.stop_requested()) {
        // 5초 sleep을 stop에 조금 더 잘 반응하게 100ms * 50으로 쪼갬(안전/권장)
        for (int i = 0; i < 50 && !st.stop_requested(); ++i) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        if (st.stop_requested()) break;

        configuration::do_realtime();
        const std::string ll = configuration::get_string(progname_, "log_level", "info");

        if (!boost::iequals(ll, log_level_)) {
            spdlog::info("{{LLC}} change loglevel from {} to {}", log_level_, ll);
            log_level_ = ll;

            spdlog::set_level(to_spdlog_level(log_level_));
        }
    }

    spdlog::info("{{LLC}} stopped");
}
