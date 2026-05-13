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

enum class RsltType : int {
    OK = 0,
    SYSFAIL = 1,
    FORMAT_ERR = 2,
    ETC = 3,
    INVALID_BODYLEN = 14,
    INVALID_CID = 15,
    DUP_CIDNO = 5,
    DUPINSERT = 10,
    DUPDELETE = 11,
    NOTROW = 12,
    INVALID_ACTION = 16
};
