#pragma once

#include <spdlog/spdlog.h>
#include <spdlog/sinks/hourly_file_sink.h>
#include <spdlog/pattern_formatter.h>


// original: daily_filename_calculator
// {}_{:04d}-{:02d}-{:02d}{}, basename, now_tm.tm_year + 1900, now_tm.tm_mon + 1, now_tm.tm_mday, ext

struct custom_hourly_filename_calculator
{
    static spdlog::filename_t calc_filename(const spdlog::filename_t &basename, const tm &now_tm)
    {
		return fmt::format(SPDLOG_FILENAME_T("{}.{:04d}{:02d}{:02d}-{:02d}"), basename, now_tm.tm_year + 1900, now_tm.tm_mon + 1, now_tm.tm_mday, now_tm.tm_hour);
    }
};


