#pragma once

#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/types.h>
#include <unistd.h>

#define LENGTH_ERROR -2
#define worker_log_pattern "%m-%d %T.%e [%L] %v"

//// End of File Indicator of KS File
typedef enum KS_EndOfFile { KSEOF = 26 } KS_EndOfFile_t;

typedef enum SystemDepanDef {
  FLK_FD_MAX = _POSIX_OPEN_MAX,
  FLK_PATH_MAX = _POSIX_PATH_MAX,
  FLK_SYS_NO_USE_DEF = 0xffffffff
} SystemDepanDef_t;

#define SYSTEM "SIMU"

#define SYSTEM_HOME SYSTEM "_HOME"
#define SYSTEM_CONFIG_HOME "SIMU__CONFIG_HOME"
#define SYSTEM_CONFIG_FILE SYSTEM "_CONFIG_FILE"
#define SYSTEM_BIN_HOME SYSTEM "_BIN_HOME"

#define SYSTEM_MASTER "sms_master"
#define SYSTEM_SCHD_FOR_ACTIVE "sms_schdA"
#define SYSTEM_SCHD_FOR_RECIEPT "sms_schdR"
#define SYSTEM_SCHD_FOR_RCS "sms_schdRCS"
#define SYSTEM_ASE "sms_ase"

/* class */
#ifdef __cplusplus
#include <iostream>
#include <string>

using namespace std;

namespace smsc {
typedef basic_string<char> String;
}

#endif
