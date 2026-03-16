#pragma once

#include <spdlog/spdlog.h>
#include <spdlog/sinks/daily_file_sink.h>
#include <spdlog/pattern_formatter.h>


// original: daily_filename_calculator
// {}_{:04d}-{:02d}-{:02d}{}, basename, now_tm.tm_year + 1900, now_tm.tm_mon + 1, now_tm.tm_mday, ext
struct custom_daily_filename_calculator
{
    static spdlog::filename_t calc_filename(const spdlog::filename_t &basename, const tm &now_tm)
    {
#if 0
        spdlog::memory_buf_t w;
        fmt::format_to(w, "{}.{:04d}{:02d}{:02d}", basename, now_tm.tm_year + 1900, now_tm.tm_mon + 1, now_tm.tm_mday);
        return fmt::to_string(w);
#endif
		return fmt::format(SPDLOG_FILENAME_T("{}.{:04d}{:02d}{:02d}"), basename, now_tm.tm_year + 1900, now_tm.tm_mon + 1, now_tm.tm_mday);
    }
};
#if 0
struct custom_hour_filename_calculator
{
    static spdlog::filename_t calc_filename(const spdlog::filename_t &basename, const tm &now_tm)
    {
#if 0
        spdlog::memory_buf_t w;
        fmt::format_to(w, "{}.{:04d}{:02d}{:02d}", basename, now_tm.tm_year + 1900, now_tm.tm_mon + 1, now_tm.tm_mday);
        return fmt::to_string(w);
#endif
		return fmt::format(SPDLOG_FILENAME_T("{}.{:04d}{:02d}{:02d}-{:02d}"), basename, now_tm.tm_year + 1900, now_tm.tm_mon + 1, now_tm.tm_mday, now_tm.tm_hour);
    }
};
#endif

#if 0
// original: daily_filename_calculator
// // {}_{:04d}-{:02d}-{:02d}{}, basename, now_tm.tm_year + 1900, now_tm.tm_mon + 1, now_tm.tm_mday, ext
struct custom_daily_filename_calculator {
	static spdlog::filename_t calc_filename(const spdlog::filename_t &basename, const tm &now_tm)
	{
#if 0
		spdlog::memory_buf_t w;
		fmt::format_to(w, "{}.{:04d}-{:02d}-{:02d}", basename, now_tm.tm_year + 1900, now_tm.tm_mon + 1, now_tm.tm_mday);
		return fmt::to_string(w);
#else
		return fmt::format(SPDLOG_FILENAME_T("{}.{:04d}-{:02d}-{:02d}"), basename, now_tm.tm_year + 1900, now_tm.tm_mon + 1, now_tm.tm_mday);
#endif
	}
};
#endif
