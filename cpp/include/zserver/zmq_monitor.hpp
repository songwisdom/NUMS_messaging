#include <iostream>
#include "loglevel_checker.hpp"
#include "spdlog/async.h"
#include "spdlog/fmt/bin_to_hex.h"
#include "spdlog/sinks/daily_file_sink.h"
#include "spdlog/spdlog.h"
#include "spdlog_custom_daily_filename_calculator.h"
#include "spdlog_custom_hourly_filename_calculator.h"
#include "zmq.hpp"

class plk_monitor_t : public zmq::monitor_t
{
	public:
		void on_monitor_started()
		{
			spdlog::info("[{}] on_monitor_started", "dual sender");
		}

		// just connected, needed handshaking, not working...
		void on_event_connected(const zmq_event_t &event, const char *addr) ZMQ_OVERRIDE {
			++connected_total_;
			spdlog::info("[{}, {}] on_event_connected, total: {}", "dual sender", addr, connected_total_);
		}

		void on_event_handshake_failed_no_detail(const zmq_event_t &, const char *addr) ZMQ_OVERRIDE {
			++handshake_failed_no_detail_; // it's a correction for on_event_disconnected
			spdlog::warn("[{}, {}] on_event_handshake_failed_no_detail, correction: {}", "dual sender", addr, handshake_failed_no_detail_);
		}

		void on_event_handshake_failed_protocol(const zmq_event_t &event, const char *addr) ZMQ_OVERRIDE {
			spdlog::warn("[{}, {}] on_event_handshake_failed_protocol", "dual sender", addr);
		}

		// ready to I/O
		void on_event_handshake_succeeded(const zmq_event_t &event, const char *addr) ZMQ_OVERRIDE {
			++connected_;
			++handshaked_total_;
			spdlog::info("[{}, {}] on_event_handshake_succeeded, current: {}, accumulated-handshaking: {}", "dual sender", addr, connected_, handshaked_total_);
		}

		void on_event_disconnected(const zmq_event_t &event, const char *addr) ZMQ_OVERRIDE {
			if (handshake_failed_no_detail_ > 0)
			{
				--handshake_failed_no_detail_;
			} else
			{
				--connected_;
				spdlog::info("[{}, {}] on_event_disconnected, current: {}, accumulated-handshaking: {}", "dual sender", addr, connected_, handshaked_total_);
			}

		}

		int connected_total_{0};
		int handshaked_total_{0};
		int handshake_failed_no_detail_{0};
        int connected_{0};

		int get_connection()
		{
			return connected_;
		}
};