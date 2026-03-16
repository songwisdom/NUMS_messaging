#pragma once

enum class MsgType : int {
    LINK = 1,
    LINK_ACK = 2,
    SN_REGINFO = 3,
    SN_REGINFO_ACK = 4,
    REQ_RESULT = 5,
    REQ_RESULT_ACK = 6,
    UNKNOWN = -1 
};

