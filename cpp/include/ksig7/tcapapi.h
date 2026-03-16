
#ifdef _DEBUG
#if 0
#define __N_ENTER_FUNC__        \
        fprintf(logfp, "LIB] enter %s,%d,%s\n", __FUNCTION__,__LINE__,__FILE__);

#define __N_QUIT_FUNC__ \
        fprintf(logfp, "LIB] quit %s,%d,%s\n", __FUNCTION__,__LINE__,__FILE__);\
        fclose(logfp);

#define __ENTER_FUNC__  \
        fprintf(logfp, "LIB] enter %s,%d,%s\n", __FUNCTION__,__LINE__,__FILE__);

#define __QUIT_FUNC__   \
        fprintf(logfp, "LIB] quit %s,%d,%s\n", __FUNCTION__,__LINE__,__FILE__);

FILE    *logfp;
char    logfilename[256];
#else
#define __N_ENTER_FUNC__        \
        fprintf(stdout, "LIB] enter %s,%d,%s\n", __FUNCTION__,__LINE__,__FILE__);

#define __N_QUIT_FUNC__ \
        fprintf(stdout, "LIB] quit %s,%d,%s\n", __FUNCTION__,__LINE__,__FILE__);\

#define __ENTER_FUNC__  \
        fprintf(stdout, "LIB] enter %s,%d,%s\n", __FUNCTION__,__LINE__,__FILE__);

#define __QUIT_FUNC__   \
        fprintf(stdout, "LIB] quit %s,%d,%s\n", __FUNCTION__,__LINE__,__FILE__);

#endif
#else
#define __N_ENTER_FUNC__
#define __N_QUIT_FUNC__
#define __ENTER_FUNC__
#define __QUIT_FUNC__
#endif


/************************************************************************************************************/
/**********************  KBenv.h Start                                                    *******************/
/************************************************************************************************************/
typedef unsigned char   Bool;           /* boolean */
typedef char            S8;             /* signed - 8 bits */
typedef unsigned char   U8;             /* unsigned - 8 bits */
#ifdef short
#undef short
typedef short           short;            /* signed - 16 bits */
#endif
typedef unsigned short  U16;            /* unsigned - 16 bits */

typedef int             S32;            /* signed - 32 bits */
typedef unsigned int    U32;            /* unsigned - 32 bits */

typedef long            S64;            /* signed - 32 bits */
typedef unsigned long   U64;            /* unsigned - 32 bits */

typedef void            Void;           /* void */
typedef Void            VOID;           /* void */

#ifdef  PUBLIC
#undef  PUBLIC
#define PUBLIC                          /* public is c default scope */
#else   /* not PUBLIC */
#define PUBLIC
#endif  /* PUBLIC */

#ifdef  PRIVATE
#undef  PRIVATE
#define PRIVATE         static          /* private is c static scope */
#else   /* not PRIVATE */
#define PRIVATE         static          /* private is c static scope */
#endif  /* PRIVATE */

#ifdef  EXTERN
#undef  EXTERN
#define EXTERN          extern
#else   /* not EXTERN */
#define EXTERN          extern
#endif  /* EXTERN */

#ifdef  ANSI
#define CONSTANT        const           /* constant */
#else
#define CONSTANT
#endif  /* ANSI */

#if     defined(DEBUG) && !defined(sun)
#define STATIC
#else
#define STATIC          static
#endif

#define RETVOID         return          /* void */
#define RETVALUE(v)     return(v)       /* return value */

#define ROK             0               /* ok */
#define RFAILED        -1               /* failed, general */

#ifndef FALSE
#define FALSE           0               /* false */
#endif
#ifndef TRUE
#define TRUE            1               /* ture */
#endif

#define NULLP           0L              /* null pointer */
#define NULLD           0L              /* null data */
#ifndef BIT_64
#define PTR             U32             /* pointer */
#define PTRSIZE         sizeof(U32)     /* pointer size */
#else   /* BIT_64 */
#define PTR             U64             /* pointer */
#define PTRSIZE         sizeof(U64)     /* pointer size */
#endif  /* BIT_64 */
/*********************************************************************************************** *************/
/**********************  KBenv.h End                                                      *******************/
/************************************************************************************************************/


/***********************************************************************************************/
/* Macro                                                                                       */
/***********************************************************************************************/
/* variable check */
#define CHK_cnxid(x)  \
    if (x < 0){                                                                 \
        fprintf(stderr, "cnxid(%d) is less than 0!!\n", x);                     \
        tc_errno = TCE_ILLEGAL_VALUE;                                           \
        RETVALUE(RFAILED);                                                      \
    }
#define CHK_tcap_service_id(x)  \
    if (x != TCX_TCAP_STD){                                                     \
        fprintf(stderr, "tcap_service_id(%d) isn't TCX_TCAP_STD !!\n", x);      \
        tc_errno = TCE_ILLEGAL_VALUE;                                           \
        RETVALUE(RFAILED);                                                      \
    }
#define CHK_ssn(x)      \
    if (x > 0xFF){                                              \
        fprintf(stderr, "ssn(%d) isn't less than 256 !!\n", x); \
        tc_errno = TCE_INVALID_SSN;                             \
        RETVALUE(RFAILED);                                      \
    }
#define CHK_enable_comp_layer(x)        \
    if (x == TC_NO){                                                            \
        fprintf(stderr, "Don't Support TR_user !!\n");                          \
        tc_errno = TCE_NO_SERVICE;                                              \
        RETVALUE(RFAILED);                                                      \
    } else if (x != TC_YES){                                                    \
        fprintf(stderr, ""#x"(%d) is TCE_ILLEGAL_VALUE !!\n", x);               \
        tc_errno = TCE_ILLEGAL_VALUE;                                           \
        RETVALUE(RFAILED);                                                      \
    }
#define CHK_active(x)   \
    if ((x != TC_YES) && (x != TC_NO)){                                 \
        fprintf(stderr, ""#x"(%d) is TCE_ILLEGAL_VALUE !!\n", x);       \
        tc_errno = TCE_ILLEGAL_VALUE;                                   \
        RETVALUE(RFAILED);                                              \
    }
#define CHK_appli_info(x)       \
    if ((x) == NULL){                                                                                   \
        fprintf(stderr, "appli_info is NULL !!\n");                                                     \
        tc_errno = TCE_ILLEGAL_VALUE;                                                                   \
        RETVALUE(RFAILED);                                                                              \
    }                                                                                                   \
    if ((x)->mode != TCX_CNX_OVERWRITE){                                                                \
        fprintf(stderr, "appli_info->mode(%d) isn't TCX_CNX_OVERWRITE !!\n", (x)->mode);                \
        tc_errno = TCE_ILLEGAL_VALUE;                                                                   \
        RETVALUE(RFAILED);                                                                              \
    }                                                                                                   \
    if (((x)->application_id < 0)||((x)->instance_id >= MAX_APP_NUM)){                                  \
        fprintf(stderr, "appli_info->application_id(%d) is less than 0 !!\n", (x)->application_id);     \
        tc_errno = TCE_WRONG_APPLICATION_ID;                                                            \
        RETVALUE(RFAILED);                                                                              \
    }                                                                                                   \
    if (((x)->instance_id < 0)||((x)->instance_id >= MAX_INST_NUM)){                                    \
        fprintf(stderr, "appli_info->instance_id(%d) is less than 0 !!\n", (x)->instance_id);           \
        tc_errno = TCE_WRONG_INSTANCE_ID;                                                               \
        RETVALUE(RFAILED);                                                                              \
    }
#define CHK_sccp_service_kind(x)        \
    if (((x&0x0003) != TCX_SCCP_SERVICE_REGULAR) &&                     \
        ((x&0x0003) != TCX_SCCP_SERVICE_ITU_WB)  &&                     \
        ((x&0x0003) != TCX_SCCP_SERVICE_ANSI96_NOISNI)){                \
        fprintf(stderr, ""#x"(%d) is TCE_ILLEGAL_VALUE !!\n", x);       \
        tc_errno = TCE_ILLEGAL_VALUE;                                   \
        RETVALUE(RFAILED);                                              \
    }                                                                   \
    if (((x&0x0300) != 0x0000)          &&                              \
        ((x&0x0300) != TCX_SCCP_NO_MGT) &&                              \
        ((x&0x0300) != TCX_SCCP_MGT_ONLY)){                             \
        fprintf(stderr, ""#x"(%d) is TCE_ILLEGAL_VALUE !!\n", x);       \
        tc_errno = TCE_ILLEGAL_VALUE;                                   \
        RETVALUE(RFAILED);                                              \
    }
#define CHK_timeout(x, default) \
    if ((x) == NULL){                   \
        tm.tv_sec  = default;           \
        tm.tv_usec = 0;                 \
    } else {                            \
        tm.tv_sec  = (x)->tv_sec;       \
        tm.tv_usec = (x)->tv_usec;      \
    }


/***********************************************************************************************/
/* Define                                                                                      */
/***********************************************************************************************/
/* Etc */
#define SET_BLOCKING            1
#define SET_NONBLOCKING         2

#define TA_MSGTYPE_BASE         0x00010000
#define TA_SHMEM_KEY            0x10002000

#define APP_CTRL_BASE_PORT      10500
#define MAX_APP_CTRL_PORT       500
#define APP_DATA_BASE_PORT      11000
#define MAX_APP_DATA_PORT       500

#define TA_SHMEM_KEY    0x10002000
#define UDP_DATA_LENGTH 8192
#define UDP_HEAD_LENGTH 8

#define MAX_CNX_NUM             100
#define MAX_APP_NUM             32
#define MAX_INST_NUM            32
//#define MAX_DIAL_NUM            2048
#define MAX_DIAL_NUM            8192
#define MAX_INV_NUM             64

/* MSG type */
#define TCGL_TCX_OPEN_MSG           (TA_MSGTYPE_BASE+0x0011)
#define TCGL_TCX_OPEN_MSG_ACK       (TA_MSGTYPE_BASE+0x0012)
#define TCGL_TCX_CLOSE_MSG          (TA_MSGTYPE_BASE+0x0013)
#define TCGL_TCX_CLOSE_MSG_ACK      (TA_MSGTYPE_BASE+0x0014)
#define TCGL_TCX_GET_DID_MSG        (TA_MSGTYPE_BASE+0x0015)
#define TCGL_TCX_GET_DID_MSG_ACK    (TA_MSGTYPE_BASE+0x0016)
#define TCGL_TCX_CTL_ACT_MSG        (TA_MSGTYPE_BASE+0x0017)
#define TCGL_TCX_CTL_ACT_MSG_ACK    (TA_MSGTYPE_BASE+0x0018)
#define TCGL_TCX_CTL_DESACT_MSG     (TA_MSGTYPE_BASE+0x0019)
#define TCGL_TCX_CTL_DESACT_MSG_ACK (TA_MSGTYPE_BASE+0x001A)
#define TCGL_TCX_HB_MSG             (TA_MSGTYPE_BASE+0x001B)
#define TCGL_TCX_HB_MSG_ACK         (TA_MSGTYPE_BASE+0x001C)

/*------- tcap <-> tcap user -------*/
#define N_PRIM_BASE             0x01
#define N_PRIM_BASE_CO          N_PRIM_BASE
#define N_INFORM_REQ           (N_PRIM_BASE_CO+0x08)
#define N_INFORM_IND            N_INFORM_REQ
#define N_PRIM_BASE_CL         (N_INFORM_IND + 1)      /* N_PRIM_BASE_CL : 0x09 */
#define N_NOTICE_IND           (N_PRIM_BASE_CL+0x01)
#define N_PRIM_BASE_MGR        (N_NOTICE_IND + 1)      /* N_PRIM_BASE_MGR : 0x0B */
#define N_PCSTATE_IND          (N_PRIM_BASE_MGR+0x03)
#define N_PRIM_BASE_SMS        (N_PCSTATE_IND + 1)     /* N_PRIM_BASE_SMS : 0x0F */
#define N_UNAVAIL_SSN          (N_PRIM_BASE_SMS+0x08)

#define TC_PRIM_BASE            (N_UNAVAIL_SSN+0xFF)  /* TC_PRIM_BASE : 0x116 */
#define TC_INVOKE_REQ           (TC_PRIM_BASE + 0x00)
#define TC_RESULT_NL_REQ        (TC_PRIM_BASE + 0x01)
#define TC_RESULT_L_REQ         (TC_PRIM_BASE + 0x02)
#define TC_U_ERROR_REQ          (TC_PRIM_BASE + 0x03)
#define TC_U_REJECT_REQ         (TC_PRIM_BASE + 0x04)
#define TC_U_CANCEL_REQ         (TC_PRIM_BASE + 0x05)

#define TC_INVOKE_IND           (TC_PRIM_BASE + 0x06)
#define TC_RESULT_NL_IND        (TC_PRIM_BASE + 0x07)
#define TC_RESULT_L_IND         (TC_PRIM_BASE + 0x08)
#define TC_U_ERROR_IND          (TC_PRIM_BASE + 0x09)
#define TC_U_REJECT_IND         (TC_PRIM_BASE + 0x0A)
#define TC_R_REJECT_IND         (TC_PRIM_BASE + 0x0B)
#define TC_L_REJECT_IND         (TC_PRIM_BASE + 0x0C)
#define TC_L_CANCEL_IND         (TC_PRIM_BASE + 0x0D)

#define TC_UNI_REQ              (TC_PRIM_BASE + 0x0E)
#define TC_BEGIN_REQ            (TC_PRIM_BASE + 0x0F)
#define TC_CONTINUE_REQ         (TC_PRIM_BASE + 0x10)
#define TC_END_REQ              (TC_PRIM_BASE + 0x11)
#define TC_U_ABORT_REQ          (TC_PRIM_BASE + 0x12)

#define TC_UNI_IND              (TC_PRIM_BASE + 0x13)
#define TC_BEGIN_IND            (TC_PRIM_BASE + 0x14)
#define TC_CONTINUE_IND         (TC_PRIM_BASE + 0x15)
#define TC_END_IND              (TC_PRIM_BASE + 0x16)
#define TC_U_ABORT_IND          (TC_PRIM_BASE + 0x17)
#define TC_P_ABORT_IND          (TC_PRIM_BASE + 0x18)

#define TC_NOTICE_IND           (TC_PRIM_BASE + 0x19)
#define TC_STATE_IND            (TC_PRIM_BASE + 0x1A)
#define TC_PCSTATE_IND          (TC_PRIM_BASE + 0x1B)
#define TC_BEGIN_TIMEOUT        (TC_PRIM_BASE + 0x1C)   /* 20050603 :: add (1 line) */

/************************************************************************************************************/
/**********************  sccpApType.h Start                                               *******************/
/************************************************************************************************************/
#define MAX_SCCP_ADDRESS_NUM            32

typedef struct {
        U8      routingInd;     /* routing indicator */
        U8      globalTitleInd; /* global indicator */
        U8      ssnInd;         /* SSN indicator */
        U8      signalingPointCodeInd;  /* Point Code indicator */
        U8      nautreAddressInd;       /* nature of address */
        U8      translationType;        /* translation type */
        U8      encodeingScheme;        /* encodeingScheme */
        U8      numberingPlan;          /* numbering Plan */
        U32     dpc;
        U8      ssn;
        U8      addressLength; /* by ysjang */
        U8      addressStr[ MAX_SCCP_ADDRESS_NUM ];/* address(string format) */
} SccpAddress_t;
/************************************************************************************************************/
/**********************  sccpApType.h End                                                 *******************/
/************************************************************************************************************/

/************************************************************************************************************/
/**********************  tcapApType.h Start                                               *******************/
/************************************************************************************************************/
#define TC_DATA_BUF_LEN    1500

#ifdef SGSN /* commented by hskang 0119 */
#define TC_REQ_BUF_MSG_LEN      4096    /* org(228) -> 4096 : by hcshin etri request */
#define TC_IND_BUF_MSG_LEN      4096    /* org(228) -> 4096 : by hcshin etri request */
#else
//#define TC_REQ_BUF_MSG_LEN    228     /* org(228) -> 4096 : by hcshin etri request */
//#define TC_IND_BUF_MSG_LEN    220     /* org(220) -> 4096 : by hcshin etri request */
#define TC_REQ_BUF_MSG_LEN      TC_DATA_BUF_LEN
#define TC_IND_BUF_MSG_LEN      TC_DATA_BUF_LEN
#endif


#ifdef SGSN
#define TC_PRIM_MSG_LEN         4096    /* Not use <- by etri request */
#else
//#define TC_PRIM_MSG_LEN         284
#define TC_PRIM_MSG_LEN         TC_DATA_BUF_LEN
#endif
#define MAX_APN                 64      /* Max Appl.context Num : added by hcshin */

#define MSGID_TCAPREQ       (0x000D0000)
#define MSGID_TCAPIND       (0x000D0001)

#define MAX_OPCODE_LEN          8       //jjang::20031209::add

/**************** TCAP Primitives **********/
typedef struct {
        S32             octetLen;
        S8              octs[MAX_APN];  /* MAX_APN = 64 */
} AppContextName;

typedef struct {    //20041105::modify
        U8              sequenceControl;
        U8              returnOption;
} ServQual_t;

typedef struct  {
        U16             primId;
        S32             dialogueId;     /* Dialogue ID          */
        AppContextName  appContextName; /* application context name */
        //20041105::modify
        //S32           serviceQuality; /* Quality Of Service   */
        ServQual_t      servQual;       /* Quality Of Service   */
        SccpAddress_t   orgAddress;     /* Originating address  */
        SccpAddress_t   dstAddress;     /* Destination address  */
        S32             userDataLength;
        U8              userData[ TC_REQ_BUF_MSG_LEN ];/* user data */
} PrimTcapUniReq;

typedef struct  {
        U16             primId;
        S32             temp;           /* for alignment        */
        AppContextName  appContextName; /* application context name */
        //20041105::modify
        //S32           serviceQuality;
        ServQual_t      servQual;       /* Quality Of Service   */
        SccpAddress_t   orgAddress;
        SccpAddress_t   dstAddress;
        S32             compPresent;
        S32             userDataLength;
        U8              userData[ TC_IND_BUF_MSG_LEN ];/* user data */
} PrimTcapUniInd;

typedef struct  {
        U16             primId;
        S32             dialogueId;
        AppContextName  appContextName; /* application context name */
        //20041105::modify
        //S32           serviceQuality;
        ServQual_t      servQual;       /* Quality Of Service   */
        SccpAddress_t   orgAddress;
        SccpAddress_t   dstAddress;
        S32             userDataLength;
        U8              userData[ TC_REQ_BUF_MSG_LEN ];/* user data */
} PrimTcapBeginReq;

typedef struct  {
        U16             primId;
        S32             dialogueId;
        AppContextName  appContextName; /* application context name */
        //20041105::modify
        //S32           serviceQuality;
        ServQual_t      servQual;       /* Quality Of Service   */
        SccpAddress_t   orgAddress;
        SccpAddress_t   dstAddress;
        S32             compPresent;
        S32             userDataLength;
        U8              userData[ TC_IND_BUF_MSG_LEN ];/* user data */
} PrimTcapBeginInd;

typedef struct  {
        U16             primId;
        S32             dialogueId;
        AppContextName  appContextName; /* application context name */
        //S32           serviceQuality;
        ServQual_t      servQual;       /* Quality Of Service   */
        SccpAddress_t   orgAddress;
        S32             userDataLength;
        U8              userData[ TC_REQ_BUF_MSG_LEN ];/* user data */
} PrimTcapContinueReq;

typedef struct  {
        U16             primId;
        S32             dialogueId;
        AppContextName  appContextName; /* application context name */
        //20041105::modify
        //S32           serviceQuality;
        ServQual_t      servQual;       /* Quality Of Service   */
        SccpAddress_t   orgAddress;
        S32             compPresent;
        S32             userDataLength;
        U8              userData[ TC_IND_BUF_MSG_LEN ];/* user data */
} PrimTcapContinueInd;

typedef struct  {
        U16             primId;
        S32             dialogueId;
        AppContextName  appContextName; /* application context name */
        //20041105::modify
        //S32           serviceQuality;
        ServQual_t      servQual;       /* Quality Of Service   */
        S32             termination;
        S32             userDataLength;
        U8              userData[ TC_REQ_BUF_MSG_LEN ];/* user data */
} PrimTcapEndReq;

typedef struct  {
        U16             primId;
        S32             dialogueId;
        AppContextName  appContextName; /* application context name */
        //20041105::modify
        //S32           serviceQuality;
        ServQual_t      servQual;       /* Quality Of Service   */
        S32             compPresent;
        S32             userDataLength;
        U8              userData[ TC_IND_BUF_MSG_LEN ];/* user data */
} PrimTcapEndInd;

typedef struct  {
        U16             primId;
        S32             dialogueId;
        AppContextName  appContextName; /* application context name */
        //20041105::modify
        //S32           serviceQuality;
        ServQual_t      servQual;       /* Quality Of Service   */
        U8              abortReason;
        S32             userDataLength;
        U8              userData[ TC_REQ_BUF_MSG_LEN ];/* user data */
} PrimTcapUAbortReq;

typedef struct  {
        U16             primId;
        S32             dialogueId;
        AppContextName  appContextName; /* application context name */
        //20041105::modify
        //S32           serviceQuality;
        ServQual_t      servQual;       /* Quality Of Service   */
        U8              abortReason;
        S32             userDataLength;
        U8              userData[ TC_IND_BUF_MSG_LEN ];/* user data */
} PrimTcapUAbortInd;

typedef struct {
        U16             primId;
        S32             dialogueId;
} PrimTcapBeginTimeout;

typedef struct  {
        U16             primId;
        S32             dialogueId;
        U8              pAbortCause;
} PrimTcapPAbortInd;

typedef struct  {
        U16             primId;
        S32             dialogueId;
        U8              reportCause;
} PrimTcapNoticeInd;

typedef struct  {
        U16             primId;
        S32             dialogueId;
        S32             invokeId;               /* invoke ID            */
        U8              temp;           /* for alignment */
        S32             operationClass;         /* operationClass of operation  */
        S32             linkedId;               /* linked ID            */
#ifndef LONG_OPCODE
        U8              operationCode;
#else
        S32             operationCodeLength;
        U8              operationCode[MAX_OPCODE_LEN];
#endif
        S32             timer;          /* time out             */
        S32             userDataLength;
        U8              userData[TC_REQ_BUF_MSG_LEN];
} PrimTcapInvokeReq;

typedef struct  {
        U16             primId;
        S32             dialogueId;
        S32             invokeId;
        U8              lastComponentInd;
        S32             linkedId;
#ifndef LONG_OPCODE
        U8              operationCode;
#else
        S32             operationCodeLength;
        U8              operationCode[MAX_OPCODE_LEN];
#endif
        S32             userDataLength;
        U8              userData[TC_IND_BUF_MSG_LEN];
} PrimTcapInvokeInd;

typedef struct  {
        U16             primId;
        S32             dialogueId;
        S32             invokeId;
        U8              temp;           /* for alignment */
#ifndef LONG_OPCODE
        U8              operationCode;
#else
        S32             operationCodeLength;
        U8              operationCode[MAX_OPCODE_LEN];
#endif
        S32             userDataLength;
        U8              userData[TC_REQ_BUF_MSG_LEN];
} PrimTcapResultLReq;

typedef struct  {
        U16             primId;
        S32             dialogueId;
        S32             invokeId;
        U8              lastComponentInd;
#ifndef LONG_OPCODE
        U8              operationCode;
#else
        S32             operationCodeLength;
        U8              operationCode[MAX_OPCODE_LEN];
#endif
        S32             userDataLength;
        U8              userData[TC_IND_BUF_MSG_LEN];
} PrimTcapResultLInd;

typedef struct  {
        U16             primId;
        S32             dialogueId;
        S32             invokeId;
        U8              temp;           /* for alignment */
#ifndef LONG_OPCODE
        U8              operationCode;
#else
        S32             operationCodeLength;
        U8              operationCode[MAX_OPCODE_LEN];
#endif
        S32             userDataLength;
        U8              userData[TC_REQ_BUF_MSG_LEN];
} PrimTcapResultNLReq;

typedef struct  {
        U16             primId;
        S32             dialogueId;
        S32             invokeId;
        U8              lastComponentInd;
        S32             sequence;
#ifndef LONG_OPCODE
        U8              operationCode;
#else
        S32             operationCodeLength;
        U8              operationCode[MAX_OPCODE_LEN];
#endif
        S32             userDataLength;
        U8              userData[TC_IND_BUF_MSG_LEN];
} PrimTcapResultNLInd;

typedef struct  {
        U16             primId;
        S32             dialogueId;
        S32             invokeId;
        U8              temp;           /* for alignment */
        U8              ecode;          /* error code           */
        S32             userDataLength;
        U8              userData[TC_REQ_BUF_MSG_LEN];
} PrimTcapUErrorReq;

typedef struct  {
        U16             primId;
        S32             dialogueId;
        S32             invokeId;
        U8              lastComponentInd;
        U8              ecode;
        S32             userDataLength;
        U8              userData[TC_IND_BUF_MSG_LEN];
} PrimTcapUErrorInd;

typedef struct  {
        U16             primId;
        S32             dialogueId;
        S32             invokeId;
        U8              temp;           /* for alignment */
        U8              problemType;    /* problem type code    */
        U8              problemCode;    /* problem code code    */
} PrimTcapURejectReq;

typedef struct  {
        U16             primId;
        S32             dialogueId;
        S32             invokeId;
        U8              lastComponentInd; /* modified by hcshin : S32->U8 */
        U8              problemType;
        U8              problemCode;
} PrimTcapURejectInd;

typedef struct  {
        U16             primId;
        S32             dialogueId;
        S32             invokeId;
        U8              temp;           /* for alignment */
} PrimTcapLCancelInd;

typedef struct  {
        U16             primId;
        S32             dialogueId;
        S32             invokeId;
        U8              temp;           /* for alignment */
} PrimTcapUCancelReq;

typedef struct  {
        U16             primId;
        S32             dialogueId;
        S32             invokeId;
        U8              lastComponentInd;       /* added by hcshin */
        U8              problemType;
        U8              problemCode;
} PrimTcapLRejectInd;

typedef struct  {
        U16             primId;
        S32             dialogueId;
        S32             invokeId;
        U8              lastComponentInd;
        U8              problemType;
        U8              problemCode;
} PrimTcapRRejectInd;

typedef struct {
        U16             primId;
} PrimTcapGen;

typedef union {
        struct {
            U16         primId;
            S32         dialogueId;
            AppContextName      appContextName; /* application context name */
            //20041105::modify
            //S32               serviceQuality;
            ServQual_t  servQual;       /* Quality Of Service   */
            //S32         userDataLength;
            //U8          userData[ TC_IND_BUF_MSG_LEN ];/* user data */
        } Gen;
        /* Indication Message */
        /* Normal Message */
        PrimTcapUniInd          UniInd;
        PrimTcapBeginInd        BeginInd;
        PrimTcapContinueInd     ContinueInd;
        PrimTcapEndInd          EndInd;
        PrimTcapUAbortInd       UAbortInd;
        PrimTcapPAbortInd       PAbortInd;
        PrimTcapNoticeInd       NoticeInd;

        /* Request Message */
        PrimTcapUniReq          UniReq;
        PrimTcapBeginReq        BeginReq;
        PrimTcapContinueReq     ContinueReq;
        PrimTcapEndReq          EndReq;
        PrimTcapUAbortReq       UAbortReq;
} PrimDialogue;

typedef union {
        struct {
            U16         primId;
            S32         dialogueId;
            S32         invokeId;
            U8          lastComponentInd;
        } Gen;
        /* Indication Message */
        PrimTcapInvokeInd       InvokeInd;
        PrimTcapResultLInd      ResultLInd;
        PrimTcapResultNLInd     ResultNLInd;
        PrimTcapUErrorInd       UErrorInd;
        PrimTcapLCancelInd      LCancelInd;
        PrimTcapURejectInd      URejectInd;
        PrimTcapRRejectInd      RRejectInd;
        PrimTcapLRejectInd      LRejectInd;

        /* Request Message */
        PrimTcapInvokeReq       InvokeReq;
        PrimTcapResultLReq      ResultLReq;
        PrimTcapResultNLReq     ResultNLReq;
        PrimTcapUErrorReq       UErrorReq;
        PrimTcapUCancelReq      UCancelReq;
        PrimTcapURejectReq      URejectReq;
} PrimComponent;

typedef union{
        struct {
            U16         primId;
            S32         dialogueId;
        } Gen;
        U8                      General;
        /* Indication Message */
        PrimTcapUniInd          UniInd;
        PrimTcapBeginInd        BeginInd;
        PrimTcapContinueInd     ContinueInd;
        PrimTcapEndInd          EndInd;
        PrimTcapUAbortInd       UAbortInd;
        PrimTcapPAbortInd       PAbortInd;
        PrimTcapNoticeInd       NoticeInd;
        PrimTcapInvokeInd       InvokeInd;
        PrimTcapResultLInd      ResultLInd;
        PrimTcapResultNLInd     ResultNLInd;
        PrimTcapUErrorInd       UErrorInd;
        PrimTcapLCancelInd      LCancelInd;
        PrimTcapURejectInd      URejectInd;
        PrimTcapRRejectInd      RRejectInd;
        PrimTcapLRejectInd      LRejectInd;

        /* Request Message */
        PrimTcapUniReq          UniReq;
        PrimTcapBeginReq        BeginReq;
        PrimTcapContinueReq     ContinueReq;
        PrimTcapEndReq          EndReq;
        PrimTcapUAbortReq       UAbortReq;
        PrimTcapInvokeReq       InvokeReq;
        PrimTcapResultLReq      ResultLReq;
        PrimTcapResultNLReq     ResultNLReq;
        PrimTcapUErrorReq       UErrorReq;
        PrimTcapUCancelReq      UCancelReq;
        PrimTcapURejectReq      URejectReq;
} TcapPrimType;

/**********************/
typedef struct  {
        U16     primId;
        S32     length;
        U8      data[TC_PRIM_MSG_LEN];
} RawMsg_t;

typedef struct  {
        U16     primId;
        S32     length;
        U8      *data;
} PRawMsg_t;

/* operation class */
#define OP_CLASS_1                      0x01
#define OP_CLASS_2                      0x02
#define OP_CLASS_3                      0x03
#define OP_CLASS_4                      0x04

/****************************************/

/*** problem Type and Code ***/
#if 0 //jjang::20031218::modify::1T0]0z 0*@L 4Y8#4Y.
#define TC_PRBM_NO              0x00    /* NO problem                   */
#define TC_PRBM_GENERAL         0x01    /* General problem              */
#define TC_PRBM_INVOKE          0x02    /* Invoke problem               */
#define TC_PRBM_RESULT          0x03    /* Return_Result problem        */
#define TC_PRBM_ERROR           0x04    /* Return_Error problem         */
#define TC_PRBM_LOCAL           0x05    /* Local Problem (EXTENDED CODE)*/
#else
#define TC_PRBM_NO              0x80    /* NO problem                   */
#define TC_PRBM_GENERAL         0x81    /* General problem              */
#define TC_PRBM_INVOKE          0x82    /* Invoke problem               */
#define TC_PRBM_RESULT          0x83    /* Return_Result problem        */
#define TC_PRBM_ERROR           0x84    /* Return_Error problem         */
#define TC_PRBM_LOCAL           0x85    /* Local Problem (EXTENDED CODE)*/
#endif

/* Coding of General Problem                            */
#define TC_GP_UNRECOGNIZED_COMPO                0x00
#define TC_GP_MISTYPED_COMPO                    0x01
#define TC_GP_BADLY_STRUCTURED_COMPO            0x02

/* Coding of Invoke Problem                             */
#define TC_IP_DUPLICATED_INVOKE_ID              0x00
#define TC_IP_UNRECOGNIZED_OP                   0x01
#define TC_IP_MISTYPED_PARAM                    0x02
#define TC_IP_RESOURCE_LIMITATION               0x03
#define TC_IP_INITIATING_RELEASE                0x04
#define TC_IP_UNRECOGNIZED_LINKED_ID            0x05
#define TC_IP_LINKED_RESPONSE_UNEXPECTED        0x06
#define TC_IP_UNEXPECTED_LINKED_OP              0x07

/* Coding of Return Result Problem                      */
#define TC_RP_UNRECOGNIZED_INVOKE_ID            0x00
#define TC_RP_RETURN_RESULT_UNEXPECTED          0x01
#define TC_RP_MISTYPED_PARAM                    0x02

/* Coding of Return Error Problem                       */
#define TC_EP_UNRECOGNIZED_INVOKE_ID            0x00
#define TC_EP_RETURN_ERROR_UNEXPECTED           0x01
#define TC_EP_UNRECOGNIZED_ERROR                0x02
#define TC_EP_UNEXPECTED_ERROR                  0x03
#define TC_EP_MISTYPED_PARAM                    0x04

/* Coding of Local Problem                              */
#define TC_LP_DUPLICATED_INVOKE_ID              0x00
#define TC_LP_UNRECOGNIZED_INVOKE_ID            0x01
#define TC_LP_UNRECOGNIZED_DIALOGUE_ID          0x02
#define TC_LP_RESOURCE_LIMITATION               0x03
#define TC_LP_PROTOCOL_ERROR                    0x04

/* Coding of P-Abort Cause values                       */
#define TR_UNRECOGNIZED_MESSAGE_TYPE            0x00
#define TR_UNRECOGNIZED_TRANSACTION_ID          0x01
#define TR_BADLY_FORMATTED_TRANSACTION_PORTION  0x02
#define TR_INCORRECT_TRANSACTION_PORTION        0x03

/* Coding of Local P-Abort Cause Values.                */
#define TR_RESOURCE_LIMITATION                  0x04
#define TR_ABNORMAL_DIALOGUE                    0x05 /* by wjcha */
#define TR_NO_COMMON_DIALOGUE_PORTION           0x06 /* added by hcshin */

/************************************************************************************************************/
/**********************  tcapApType.h End                                                 *******************/
/************************************************************************************************************/

/***********************************************************************************************/
/* Structs                                                                                     */
/***********************************************************************************************/
typedef struct {
        S32     msgtype;        /* message type         */
        S32     datalen;
        U8      userdata[UDP_DATA_LENGTH - UDP_HEAD_LENGTH];  /* user data */
} udp_msg_struct;

typedef struct {
    U32         ip;
    U16         port;
} ipAddr_t;

typedef struct {
    S32 flag;
    S32 cnxid;
    S32 enable_comp_layer;
    S32 rfd;
    S32 wfd;
    S32 efd;
    S32 cfd;
    ipAddr_t            c_addr;
    ipAddr_t            d_addr;
    struct timeval      tm;
    S32 more;

    pthread_t   hb_thrId;
} cnx_info_t;

typedef struct {
    S32 flag;
    S32 cnxid;
    U32 user_dialogue_id;
    S32 provider_dialogue_id;
    tcx_component *recv_componentp;
    tcx_component *send_componentp;
    S32 end_flag;
    S32 com_pres;
    S32 lcom_flag;

    tc_address  d_address;
    tc_address  o_address;
    S32 nmb_scomp;
    tcx_component *scompp[MAX_INV_NUM];
    tcx_component rcompp[5];
} dial_info_t;

typedef struct {
    S32 mode;
    S32 application_id;
    S32 instance_id;
} tcgl_app_info_t;

/* API <-> TCGL */
typedef struct {
    S32 tcap_service_id;
    S32 ssn;
    S32 enable_comp_layer;
    S32 active;
    tcgl_app_info_t appli_info;
    S32 sccp_service_kind;
    U16 data_port;
} tcgl_tcx_open_t;

typedef struct {
    tcx_cnxid   cnxid;
} tcgl_tcx_open_ack_t;

typedef struct {
    tcx_cnxid   cnxid;
} tcgl_tcx_close_t;

typedef struct {
    S32 flag;
} tcgl_tcx_close_ack_t;

typedef struct {
    U32 user_dialogue_id;
} tcgl_tcx_get_did_t;

typedef struct {
    S32 provider_dialogue_id;
} tcgl_tcx_get_did_ack_t;

typedef struct {
    tcx_cnxid   cnxid;
} tcgl_tcx_ctl_act_t;

typedef struct {
    tcx_cnxid   cnxid;
} tcgl_tcx_ctl_act_ack_t;

typedef struct {
    tcx_cnxid   cnxid;
} tcgl_tcx_ctl_desact_t;

typedef struct {
    tcx_cnxid   cnxid;
} tcgl_tcx_ctl_desact_ack_t;

typedef struct {
    tcx_cnxid   cnxid;
} tcgl_tcx_hb_t;

typedef struct {
    tcx_cnxid   cnxid;
} tcgl_tcx_hb_ack_t;

/* shmem struct */
typedef struct {
    S32 local_avail;
    S32 mate_avail;
    ipAddr_t    local_ctrlAddr;
    ipAddr_t    mate_ctrlAddr;
    ipAddr_t    local_dataAddr;
    ipAddr_t    mate_dataAddr;
} ipAddrInfo_t;

typedef struct {
    ipAddrInfo_t ipAddrInfo;
} TcapApiInfoShm_t;

typedef struct rbufferp_struct{
    struct rbufferp_struct *next_bufferp;
    tcx_primitive       *primitive;
    tcx_component       *component;
} rbufferp_t;

