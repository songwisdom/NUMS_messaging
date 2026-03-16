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
#include "rapidjson/writer.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/document.h"

typedef enum {
    LINK = 1,
    LINK_ACK,
    SN_REGINFO,
    SN_REGINFO_ACK,
    REQ_RESULT,
    REQ_RESULT_ACK
} eHeaderMsgType;

#define NUMS_H_MSG_TYPE_LEN 3
#define NUMS_H_MSG_BODY_LEN 6
#define NUMS_H_ACTION_CODE_LEN 2
#define NUMS_H_SEQ_NUM_LEN 13
#define NUMS_CID_LEN 12
#define NUMS_TIME_LEN 11
#define NUMS_SERVICE_NAME_LEN 32
#define NUMS_INFO_RATE_LEN 11
#define NUMS_BODY_COMMON_LEN 2
#define NUMS_RESERVED_LEN 3

typedef struct {
    char msg_type[NUMS_H_MSG_TYPE_LEN];
    char action_code[NUMS_H_ACTION_CODE_LEN];
    char cid[NUMS_CID_LEN];
    char s_time[NUMS_TIME_LEN];
    char e_time[NUMS_TIME_LEN];
    char service_name[NUMS_SERVICE_NAME_LEN];
    char info_rate[NUMS_INFO_RATE_LEN];
    char body_msg_type[NUMS_BODY_COMMON_LEN];
    char bill_type[NUMS_BODY_COMMON_LEN];
    char bill_limit[NUMS_BODY_COMMON_LEN];
}nums_msg;

#define NUMS_NAME_MSG_TYPE "msg_type"
#define NUMS_NAME_ACTION_CODE "action_code"
#define NUMS_NAME_S_TIME "s_time"
#define NUMS_NAME_CID "cid"
#define NUMS_NAME_E_TIME "e_time"
#define NUMS_NAME_SVC_NAME "service_name"
#define NUMS_NAME_INFO_RATE "info_rate"
#define NUMS_NAME_BODY_MSG_TYPE "body_msg_type"
#define NUMS_NAME_BILL_TYPE "bill_type"
#define NUMS_NAME_BILL_LIMIT "bill_limit"

bool init(int argc, char **argv);
