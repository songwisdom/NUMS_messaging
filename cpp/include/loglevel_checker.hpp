#pragma once
#include <string>
#include <stop_token>

class loglevel_checker {
public:
    loglevel_checker(std::string progname, std::string log_level)
        : progname_(std::move(progname)), log_level_(std::move(log_level)) {}

    void start(std::stop_token st);

private:
    std::string progname_;
    std::string log_level_;
};
