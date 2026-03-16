/***********************************************************
*
* $Header: /home/cvs/smsc/skt-smsc/include/ksig7/ttlCommon.h,v 1.1 2008/07/21 05:12:48 prowoni Exp $
*
* $Source: /home/cvs/smsc/skt-smsc/include/ksig7/ttlCommon.h,v $
* $Revision: 1.1 $
* $Locker:  $
*
***********************************************************/

/**************************************************************************
*
* This file should be included in your file via #include <ainCommon.h>
*
**************************************************************************/

#ifndef TTLCOMMON_H
#define TTLCOMMON_H


#ifdef _WIN32
#  ifndef HPAIN_TTLAPI
#      define HPAIN_TTLAPI __declspec(dllimport)
#  endif
#else
#define HPAIN_TTLAPI extern
#endif /* _WIN32 */

#define TELECOM_SCP 62
#define TELECOM_SS7 63

extern int   TTL_V_threadId;

typedef enum {
   TTL_E_SAME_THREAD = 0,
   TTL_E_NEW_THREAD  = 1,
   TTL_E_NO_THREAD   = 2
} TTL_T_threadControl;


/**************************************************************************
 * Different Levels for Tracing
 **************************************************************************/

/*
 * Trace Levels
 */
typedef enum {

    COM_E_LL_FUNCTION,        /* Function (or method) flow; entry, middle
                                 and/or exit: Verbose */
    COM_E_LL_MEMORY,          /* Heap memory allocation/deallocation; for
                                 data structures: Limited */
    COM_E_LL_OBJECTS,         /* Object instantiation/destruction:
                                 Verbose */
    COM_E_LL_ERROR,           /* Exception handling path (exception was
                                 detected): Limited */
    COM_E_LL_INFOFLOW1,       /* Message/PDU/Event flow through (basic level):
                                 Limited */
    COM_E_LL_INFOFLOW2,       /* Message/PDU/Event flow through (full level):
                                 Verbose */
    COM_E_LL_STATES,          /* State changes: 
				 Verbose */
    COM_E_LL_STARTUP,         /* Initialization or other startup statuses
                                 (for example, config info) :Verbose (but limited in
                                 time) */
    lgend } TTL_T_TraceLevel;
/*
 * the following typedef has been included for backward compatibility
 */
typedef TTL_T_TraceLevel COM_T_logLevel;

typedef enum {
    TTL_E_LOG_INFORMATIVE = 0x00000001,
    TTL_E_LOG_WARNING     = 0x00000002,
    TTL_E_LOG_ERROR       = 0x00000004,
    TTL_E_LOG_DISASTER    = 0x00000008
} TTL_T_LogLevel;

/* 
 * Functional Areas 
 */
typedef enum {
     COM_E_FA_FT_LIB     = 0x00000001,
     COM_E_FA_EH_LIB     = 0x00000002,
     COM_E_FA_MIB_LIB    = 0x00000004,  /* Mib API Library or Mib Framework */
     COM_E_FA_SS7_LIB    = 0x00000008,
     COM_E_FA_UTIL_LIB   = 0x00000010,
     COM_E_FA_DDL_LIB    = 0x00000020,
     COM_E_FA_SMS_LIB    = 0x00000040,
     COM_E_FA_SCE        = 0x00000080,
     COM_E_FA_MB_LIB     = 0x00000100,	/* Mib Schema Library */
     COM_E_FA_PPP_LIB 	 = 0x00000200,
     COM_E_FA_MSG_LIB    = 0x00000400,
     COM_E_FA_BF_LIB     = 0x00000800,	/* Babel fish lib */
     COM_E_FA_ISUP_LIB   = 0x00001000,  /* ISUP lib */
     COM_E_FA_MASH_SIMGR_LIB = 0x00002000,  /* MASH Management */
     COM_E_FA_MASH_SIDC_LIB  = 0x00004000,  /* MASH Data */
     COM_E_FA_SIL_LIB 	 = 0x00008000,
     COM_E_FA_LIB_MASK 	 = 0x0000ffff,

     COM_E_FA_reserved_7    = 0x00010000,
     COM_E_FA_reserved_8    = 0x00020000,
     COM_E_FA_reserved_9    = 0x00040000,
     COM_E_FA_reserved_MASK = 0x00070000,

     COM_E_FA_toBeDefined_12    = 0x00080000,
     COM_E_FA_toBeDefined_11    = 0x00100000,
     COM_E_FA_toBeDefined_10    = 0x00200000,
     COM_E_FA_toBeDefined_9     = 0x00400000,
     COM_E_FA_toBeDefined_8     = 0x00800000,
     COM_E_FA_toBeDefined_7     = 0x01000000,
     COM_E_FA_toBeDefined_6     = 0x02000000,
     COM_E_FA_toBeDefined_5     = 0x04000000,
     COM_E_FA_toBeDefined_4     = 0x08000000,
     COM_E_FA_toBeDefined_3     = 0x10000000,
     COM_E_FA_toBeDefined_2     = 0x20000000,
     COM_E_FA_toBeDefined_1     = 0x40000000,
     COM_E_FA_toBeDefined_MASK  = 0x7ff80000
  } TTL_T_functionalArea;

/*
 * the following typedef has been included for backward compatibility
 */
typedef  TTL_T_functionalArea    COM_T_functionalArea;

#define COM_E_FA_default         COM_E_FA_toBeDefined_1

/*
 *	LOG Entities definition and catalog set id affectation
 */


/*  FA already defined, 	MsgSetIds range from 20 to .... */

#define     FT_LOGENTITY		20		/* FT_LIB 	*/
#define     EH_LOGENTITY  		21		/* EH_LIB	*/
#define     MIB_LOGENTITY 		22		/* MIB_LIB	*/
#define     SS7_LOGENTITY 		23		/* SS7_LIB 	*/
#define     UTIL_LOGENTITY		24		/* UTIL_LIB	*/
#define     DDL_LOGENTITY		25		/* DDL_LIB	*/
#define     SMS_LOGENTITY		26		/* SMS_LIB	*/
#define     SCE_LOGENTITY		27		/* SCE		*/
#define     MB_LOGENTITY		28		/* MB_LIB	*/
#define     PPP_LOGENTITY		29		/* PPP_LIB	*/
#define	    BF_LOGENTITY		30		/* Babel fish lib */
#define	    ISUP_LOGENTITY		31		/* ISUP lib */
#define	    NAL_LOGENTITY	        32              /* NAL for MASH lib */
#define	    SIL_LOGENTITY	        33              /* SIL_LIB */
#define	    DSS_LOGENTITY	        34              /* DSS Service */

typedef enum {
   TTL_E_LOG_ARGTYPE_END = -1,
   TTL_E_LOG_ARGTYPE_INT = 0,
   TTL_E_LOG_ARGTYPE_STR = 1
} TTL_T_LOG_ARGTYPE;
/*
 *	Trace Id definition for  TTL_M_NTRACE macro
 */
typedef enum 
{
    COM_C_TRACE_ID_00, /* reserved */

    COM_C_TRACE_ID_01,
    COM_C_TRACE_ID_02,
    COM_C_TRACE_ID_03,
    COM_C_TRACE_ID_04,
    COM_C_TRACE_ID_05,
    COM_C_TRACE_ID_06,
    COM_C_TRACE_ID_07,
    COM_C_TRACE_ID_08,
    COM_C_TRACE_ID_09,
    COM_C_TRACE_ID_10,

    COM_C_TRACE_ID_11,
    COM_C_TRACE_ID_12,
    COM_C_TRACE_ID_13,
    COM_C_TRACE_ID_14,
    COM_C_TRACE_ID_15,
    COM_C_TRACE_ID_16,
    COM_C_TRACE_ID_17,
    COM_C_TRACE_ID_18,
    COM_C_TRACE_ID_19,
    COM_C_TRACE_ID_20,

    COM_C_TRACE_ID_21,
    COM_C_TRACE_ID_22,
    COM_C_TRACE_ID_23,
    COM_C_TRACE_ID_24,
    COM_C_TRACE_ID_25,
    COM_C_TRACE_ID_26,
    COM_C_TRACE_ID_27,
    COM_C_TRACE_ID_28,
    COM_C_TRACE_ID_29,
    COM_C_TRACE_ID_30,

    COM_C_TRACE_ID_31,
    COM_C_TRACE_ID_32,
    COM_C_TRACE_ID_33,
    COM_C_TRACE_ID_34,
    COM_C_TRACE_ID_35,
    COM_C_TRACE_ID_36,
    COM_C_TRACE_ID_37,
    COM_C_TRACE_ID_38,
    COM_C_TRACE_ID_39,
    COM_C_TRACE_ID_40,

    COM_C_TRACE_ID_41,
    COM_C_TRACE_ID_42,
    COM_C_TRACE_ID_43,
    COM_C_TRACE_ID_44,
    COM_C_TRACE_ID_45,
    COM_C_TRACE_ID_46,
    COM_C_TRACE_ID_47,
    COM_C_TRACE_ID_48,
    COM_C_TRACE_ID_49,
    COM_C_TRACE_ID_50,

    COM_C_TRACE_ID_SIZE
} COM_T_TRACE_ID ;


/**************************************************************************
*
*  The "reserved" enums must NOT be changed!
*
*  The "toBeDefined" enums are for each executable.  For example, the
*  SLEE group would do something like:
*
*    #define SLE_C_FA_general     COM_E_FA_toBeDefined_1
*    #define SLE_C_FA_callCollect COM_E_FA_toBeDefined_2
*    #define SLE_C_FA_smsHandler  COM_E_FA_toBeDefined_3
*    #define SLE_C_FA_dataBase    COM_E_FA_toBeDefined_4
*    #define SLE_C_FA_...         COM_E_FA_toBeDefined_...
*    }
*
*  for their own needs, as appropriate.
*
**************************************************************************/

/**************************************************************************
*
*  The following are the starting points for each group to define
*  specific types of data structures that will be dumped.  This can be
*  used either for post-processing filters (the DTRACE type will be
*  printed out by the DTRACE macro), or for some other uses.
*
**************************************************************************/

#define  TTL_C_DTRACE_MASK            0x000fffff
#define  TTL_C_DTRACE_SCP_firstType   0x00010000
#define  TTL_C_DTRACE_SLEE_firstType  0x00020000
#define  TTL_C_DTRACE_7UP_firstType   0x00030000
#define  TTL_C_DTRACE_SCE_firstType   0x00040000
#define  TTL_C_DTRACE_NMGR_firstType  0x00050000

/**************************************************************************
*
*  For example, the SLEE group would do something like:
*
*  typedef enum {SLE_E_callStruct = COM_C_DTRACE_SLEE_firstType,
*    SLE_E_otherStruct,
*    SLE_E_FA_...
*    }
*
*  in order to define their internal data structure types.
*  You can then invoke the tracer via a line such as:
*   TTL_M_DTRACE(COM_E_LL_FUNCTION,SLE_C_FA_general,"toto",SLE_E_callStruct,myPtr,10)
*
**************************************************************************/

/**************************************************************************
*
* TTL_C_FA_DEFAULT_MASK:
*
*  If the DEBUG compile flag is set, the default behaviour is to turn
*  everything on.  You can override this default behaviour either by
*  explicitly setting the different masks (via a configuration file, for
*  example), or by defining TTL_C_FA_DEFAULT_MASK before including this
*  file.
*
* TTL_M_TRACE:
*
*  This macro can be called to trace, based on a Trace Level mask and a
*  Functional Area mask.
*
*  The current behaviour is to call printf with some pre-defined
*  strings and the printf parameters you supply.  This will eventually
*  be changed to work with nettl.
*
* TTL_M_DTRACE:
*
*  This macro can be called to trace based on a Trace Level mask and a
*  Functional Area mask.
*
*  The current behaviour is to call printf with some pre-defined
*  strings, the string you supply, and a hex dump of the data structure
*  passed in.  This will eventually be changed to work with nettl.
*
**************************************************************************/

#ifdef DEBUG
/* 
 * should not enable undefined and reserved bits for funcArea here !!
 */
#ifndef TTL_C_FA_DEFAULT_MASK
#define TTL_C_FA_DEFAULT_MASK 0xffffffff
#endif /* TTL_C_FA_DEFAULT_MASK */
#else
#ifndef TTL_C_FA_DEFAULT_MASK
#define TTL_C_FA_DEFAULT_MASK 0
#endif /* TTL_C_FA_DEFAULT_MASK */
#endif /* DEBUG */

#ifdef __cplusplus
extern "C" {
#endif

HPAIN_TTLAPI COM_T_uint32    COM_isEnableLog[];
HPAIN_TTLAPI unsigned int    TTL_V_subsysId;
HPAIN_TTLAPI int             TTL_V_NTRACE_Config[];

#ifdef __cplusplus
}
#endif

/*******************************************************************/
/*  TTL_PRIV_M_TEST_TRACE_FLAG(P_level, P_funcArea)                */
/*                  ***** FOR trace macros INTERNAL USE ONLY       */
/*  DESCRIPTION:  A macro for testing the trace flags (deciding    */
/*                if a trace is to be outputed.                    */
/*                                                                 */
/*  USES:         Internally by trace macros.                      */
/*                                                                 */
/*  PARAMETERS:                                                    */
/*                                                                 */
/*  P_level      The P_level passed with the macro.                */
/*                                                                 */
/*  P_funcArea  The P_funcArea passed with the macro.            */
/*                                                                 */
/*******************************************************************/
#ifndef TTL_PRIV_M_TEST_TRACE_FLAG
#define TTL_PRIV_M_TEST_TRACE_FLAG(P_level, P_funcArea) \
   if ( COM_isEnableLog[P_level] & P_funcArea ) 
#endif /* TTL_PRIV_M_TEST_TRACE_FLAG */



/*******************************************************************/
/*  TTL_M_INIT(E_Name)                                             */
/*                                                                 */
/*                                                                 */
/*  DESCRIPTION: Initialize trace masks from debug.conf file       */
/*               Must be used in each main()                	   */
/*                                                                 */
/*  USES:         Externally                                       */
/*                                                                 */
/*  PARAMETERS: E_Name: executable name                            */
/*                                                                 */
/*                                                                 */
/*******************************************************************/
#ifndef TTL_M_INIT
#define TTL_M_INIT(E_Name)    \
{                                             \
  TTL_V_subsysId = TELECOM_SCP;            \
  scp_m_init(E_Name);                         \
}
/*
 * the following typedef has been included for backward compatibility
 */
#define  SCP_M_INIT            TTL_M_INIT
#endif /* TTL_M_INIT */





/*******************************************************************/
/*	Macros for TTL_log function (ease of use)                 */
/*******************************************************************/

#ifndef TTL_M_INT
#define TTL_M_INT(P_integerValue) 	TTL_E_LOG_ARGTYPE_INT, P_integerValue
#endif /* TTL_M_INT */
#ifndef TTL_M_STR 
#define TTL_M_STR(P_string) 		TTL_E_LOG_ARGTYPE_STR, P_string
#endif /* TTL_M_STR */
#ifndef TTL_M_EOA
#define TTL_M_EOA TTL_E_LOG_ARGTYPE_END, __LINE__, __FILE__
#endif /* TTL_M_EOA */

/*******************************************************************/
/*  TTL_M_TRACE(P_level,P_funcArea,P_exp)                          */
/*                                                                 */
/*                                                                 */
/*  DESCRIPTION: call either printf functions or nettl to record   */
/*               a trace from source code.               	   */
/*                                                                 */
/*  USES:         Externally                                       */
/*                                                                 */
/*  PARAMETERS: P_level: trace level                               */
/*              P_funcArea: functional area                        */
/*              P_exp: expression to be printed or recorded        */
/*******************************************************************/
#ifndef TTL_M_TRACE
#define TTL_M_TRACE(P_level,P_funcArea,P_exp)  \
{ \
    TTL_PRIV_M_TEST_TRACE_FLAG(P_level, P_funcArea) \
    { \
	  char *L_str; \
	  L_str = TTL_sprintf P_exp; \
	  TTL_trace(P_level, P_funcArea, L_str, __LINE__, __FILE__); \
    } \
}
/*
 * the following typedef has been included for backward compatibility
 */
#define SCP_M_TRACE        TTL_M_TRACE
#endif /* TTL_M_TRACE */

/*************************************************************************/
/*  TTL_M_DTRACE(P_level,P_funcArea,P_str,P_typeId,P_dataPtr,P_dataLen)  */
/*                                                                       */
/*                                                                       */
/*  DESCRIPTION: call either printf functions or nettl to record         */
/*               a trace from source code. Allow data "dumping"          */
/*                                                                       */
/*  USES:         Externally                                             */
/*                                                                       */
/*  PARAMETERS: P_level: trace level                                     */
/*              P_funcArea: functional area                              */
/*              P_str: expression to be printed or recorded              */
/*  		P_typeId: data type Id (to be dumped)                    */
/*		P_dataPtr: Ptr to the datat to be dumped                 */
/*		P_dataLen: length of the data to be dumped               */
/*************************************************************************/
#ifndef TTL_M_DTRACE
#define TTL_M_DTRACE(P_level,P_funcArea,P_str,P_typeId,P_dataPtr,P_dataLen)  \
{                                                                            \
  TTL_PRIV_M_TEST_TRACE_FLAG(P_level, P_funcArea) \
  { \
    TTL_dtrace(P_level, P_funcArea, P_str,P_typeId,P_dataPtr,P_dataLen, __LINE__, __FILE__); \
  } \
}
/*
 * the following typedef has been included for backward compatibility
 */
#define SCP_M_DTRACE       TTL_M_DTRACE
#endif /* TTL_M_DTRACE */



/*******************************************************************/
/*  TTL_M_NTRACE (P_level, P_funcArea, trace_id, P_exp)         */
/*                                                                 */
/*  DESCRIPTION:  An enhanced version of TTL_M_TRACE allowing      */
/*                                                                 */
/*             	  b) a scaling factor allowing the user to pass    */
/*                   a parameter which is used to scale that       */
/*                   particular trace; that is, the trace is only  */
/*                   output once for the 'scale' number of times   */
/*                   the program passes this point.                */
/*                                                                 */
/*  PARAMETERS:                                                    */
/*                                                                 */
/*******************************************************************/
#ifndef TTL_M_NTRACE
#define TTL_M_NTRACE(P_level, P_funcArea, trace_id, P_exp) \
{  \
    TTL_PRIV_M_TEST_TRACE_FLAG(P_level, P_funcArea) \
    { \
        static int    trace_count = 0 ;   \
        static int    total_count = 0 ; \
		char *L_str; \
        L_str = TTL_sprintf P_exp; \
		TTL_ntrace(P_level, P_funcArea, trace_id, L_str, &trace_count, &total_count, __LINE__, __FILE__); \
    } \
}

/*
 * the following typedef has been included for backward compatibility
 */
#define SCP_M_NTRACE       TTL_M_NTRACE
#endif /* TTL_M_NTRACE */



/* TRACE IDENTITIES -------- use with TTL_M_NTRACE------------------------------*/
/*										*/
/*Identities used to uniquely identify each trace when required.  This can be	*/
/*     used to configure the NTRACE as required.				*/
/*										*/
/*Here is an example from Slee:							*/
/*										*/
/*	#define SLE_C_TID_smscreate        COM_C_TRACE_ID_01			*/
/*	#define SLE_C_TID_smscreate_1      COM_C_TRACE_ID_02			*/
/*	#define SLE_C_TID_trace_test_1     COM_C_TRACE_ID_03			*/
/*	#define SLE_C_TID_trace_test_2     COM_C_TRACE_ID_04			*/
/*										*/
/*Then in your debug.conf file, your can affect a sampling value for each Id.	*/
/*										*/
/*------------------------------------------------------------------------------*/

/*******************************************************************/
/*                                                                 */
/* MACRO: TTL_M_NDTRACE(P_level, P_funcArea, trace_id, descr,   */
/*                      addr, length, element_size)                */
/*                                                                 */
/*  Parameters                                                     */
/*  ==========                                                     */
/*  P_level      as before                                         */
/*                                                                 */
/*  P_funcArea  as before                                         */
/*                                                                 */
/*  Scale        A scaling factor defining how often a trace is to */
/*               be output.                                        */
/*                                                                 */
/*  Tag          See NTRACE                                        */
/*                                                                 */
/*  Descr        Description of what is being output               */
/*                                                                 */
/*  Addr         The address of the item to be dumped              */
/*                                                                 */
/*  Length       The size of the elements to be dumped             */
/*                                                                 */
/*  Element_size The number of elements to be dumped               */
/*                                                                 */
/*******************************************************************/
#ifndef TTL_M_NDTRACE
#define TTL_M_NDTRACE(P_level, P_funcArea, trace_id, descr, addr, length, element_size) \
{ \
    TTL_PRIV_M_TEST_TRACE_FLAG(P_level, P_funcArea) \
    { \
        static int    trace_count = 0 ;   \
        static int    total_count = 0 ; \
        char *L_str; \
        L_str = TTL_sprintf descr; \
        TTL_ndtrace(P_level, P_funcArea, trace_id, L_str, addr, length, element_size, \
					&trace_count, &total_count, __LINE__, __FILE__); \
    }\
}
/*
 * the following typedef has been included for backward compatibility
 */
#define SCP_M_NDTRACE       TTL_M_NDTRACE
#endif /* TTL_M_NDTRACE */

/*******************************************************************/
/*                                                                 */
/* MACRO: TTL_M_ITTRACE(P_level, P_funcArea, trace_id)          */
/*                                                                 */
/*  Description                                                    */
/*  ===========                                                    */
/*                                                                 */
/*     This trace initialises a static global variable (that is,   */
/*     visable)                                                    */
/*     within the source file where this inlude file included)     */
/*     with the current time. The TTL_M_RTTRACE then read the      */
/*     timer and displays the total time and elapased timer        */
/*     relative to this start time.                                */
/*                                                                 */
/*  Parameters                                                     */
/*  ==========                                                     */
/*                                                                 */
/*     P_level, P_funcArea,      as for SCP_M_TRACE              */
/*                                                                 */
/*     trace_id     used to configure the frequency of             */
/*                  initialisation of                              */
/*                  timers, to synchronize them with               */
/*                 for example) SCP_M_NTRACE                       */
/*                                                                 */
/*******************************************************************/
#ifndef TTL_M_ITTRACE
#define TTL_M_ITTRACE(P_level, P_funcArea, trace_id) \
{  \
    TTL_PRIV_M_TEST_TRACE_FLAG(P_level, P_funcArea) \
    { \
        static int    trace_count = 0 ;   \
        static int    total_count = 0 ; \
		TTL_ittrace(P_level, P_funcArea, trace_id, &trace_count, &total_count, __LINE__, __FILE__); \
    } \
}
/*
 * the following typedef has been included for backward compatibility
 */
#define SCP_M_ITTRACE       TTL_M_ITTRACE
#endif /* TTL_M_ITTRACE */

#ifndef TTL_M_RTTRACE
#define TTL_M_RTTRACE(P_level, P_funcArea, trace_id) \
{  \
    TTL_PRIV_M_TEST_TRACE_FLAG(P_level, P_funcArea) \
    { \
        static int    trace_count = 0 ;   \
        static int    total_count = 0 ; \
        TTL_rttrace(P_level, P_funcArea, trace_id, &trace_count, &total_count, __LINE__, __FILE__); \
    } \
}

/*
 * the following typedef has been included for backward compatibility
 */
#define SCP_M_RTTRACE       TTL_M_RTTRACE
#endif /* TTL_M_RTTRACE */


/* -------------------------------------------------------------------- */
/* BEGINNING OF CIRCULAR BUFFER IMPLEMENTATION 							*/
/* -------------------------------------------------------------------- */

/* *** C
// --------------------------------------------------------------------
// TTL_C_maxBuffer is the maximum allowed circular buffer for one process.
// TTL_C_printoutStringSize is the size of output string given by the caller
// when allocating a new circular buffer.
// On error, all routines return TTL_C_error.
// --------------------------------------------------------------------
****** */

typedef	int TTL_T_histId;

						/* Number of entries is fixed ( no allocation  	*/
						/* while process is running ).                  */
#define TTL_C_maxBuffer					100
#define TTL_C_printoutStringSize		63
						/* Return values.								*/
#define TTL_C_error						-1
#define TTL_C_success					0


#ifdef __cplusplus
extern "C" {
#endif

/* *** C
// ----------------------------------------------------------
// TTL_circBuffAlloc : 
//   Allocates a circular buffer of P_nbOfRecords records, each one being
//   of P_sizeOfRecord+TTL_V_sizeOfTimeStamp size. P_string allows for more
//   friendly printout and/or dump analysis. This string is truncated if needed
//   to fit in TTL_V_printoutStringSize. An identifier TTL_T_histId is returned,
//   which should be used in any forthcoming call.
// ----------------------------------------------------------
****** */

#if defined (__STDC__) || defined (__cplusplus)
TTL_T_histId   TTL_circBuffAlloc ( size_t, int, char* );
#else
TTL_T_histId   TTL_circBuffAlloc ( );
#endif


/* *** C
// ----------------------------------------------------------
// TTL_circBuffWrite :
//   Will write  P_record in the circular  buffer  identified by histId.
//   It is the user's  responsability  to provide  the correct kind of record for
//   the named  buffer.  A timestamp is added by TTL.
//   NO CONTROL is done on sizeof P_record. It is the user's
//   responsability to ensure that P_record fits in the allocated buffer.
//   An error on size will lead to a memory fault (and death of process).
// ----------------------------------------------------------
****** */

#if defined (__STDC__) || defined (__cplusplus)
int TTL_circBuffWrite ( TTL_T_histId, void*, size_t );
#else
int TTL_circBuffWrite ( );
#endif

/* *** C
// ----------------------------------------------------------
// TTL_circBuffFlush :
//   Will flush to nettl/screen the circular buffer named by histId.  If
//   histId == 0, all circular buffers managed by TTL will be flushed.
// ----------------------------------------------------------
****** */

#if defined (__STDC__) || defined (__cplusplus)
int TTL_circBuffFlush ( TTL_T_histId );
#else
int TTL_circBuffFlush ( );
#endif


/* *** C
// ----------------------------------------------------------
// TTL_circBuffFree :
//    release allocated circular buffer. If histId == 0, all 
//    circular buffers managed by TTL will be released.
// ----------------------------------------------------------
****** */

#if defined (__STDC__) || defined (__cplusplus)
int TTL_circBuffFree ( TTL_T_histId );
#else
int TTL_circBuffFree ( );
#endif


/* *** C
// ----------------------------------------------------------
// TTL_circBuffGetPtr :
//   Allow  symbolic  dumping  by xdb of the  buffer  on the live
//   system or core dump. 
// ----------------------------------------------------------
****** */

#if defined (__STDC__) || defined (__cplusplus)
void *TTL_circBuffGetPtr ( TTL_T_histId );
#else
void *TTL_circBuffGetPtr ( );
#endif

#ifdef __cplusplus
}
#endif



/* -------------------------------------------------------------------- */
/* END OF CIRCULAR BUFFER IMPLEMENTATION 								*/
/* -------------------------------------------------------------------- */


/*********************************************************************
 *
 *	macros defined here are to improve ease of use (easy to delete
 *	in code by adding a X in front of the macro name !)
 *	Already used in Slee.
 *	(antidote)
 *********************************************************************/
#ifndef XSCP_M_TRACE
#define XSCP_M_TRACE(P_level, P_funcArea, P_exp)
#endif
#ifndef XTTL_M_TRACE
#define XTTL_M_TRACE(P_level, P_funcArea, P_exp)
#endif
#ifndef XTTL_M_NTRACE
#define XTTL_M_NTRACE(P_level, P_funcArea, trace_id, P_exp)
#endif
#ifndef XSCP_M_NTRACE
#define XSCP_M_NTRACE(P_level, P_funcArea, trace_id, P_exp)
#endif
#ifndef XSCP_M_DTRACE
#define XSCP_M_DTRACE(P_level,P_funcArea,P_str,P_typeId,P_dataPtr,P_dataLen) 
#endif
#ifndef XTTL_M_DTRACE
#define XTTL_M_DTRACE(P_level,P_funcArea,P_str,P_typeId,P_dataPtr,P_dataLen) 
#endif
#ifndef XTTL_M_NDTRACE
#define XTTL_M_NDTRACE(P_lv, P_fa, trace_id, descr, addr, length, element_size)
#endif
#ifndef XSCP_M_NDTRACE
#define XSCP_M_NDTRACE(P_lv, P_fa, trace_id, descr, addr, length, element_size)
#endif
#ifndef XTTL_M_ITTRACE 
#define XTTL_M_ITTRACE(P_level, P_funcArea, trace_id)
#endif
#ifndef XSCP_M_ITTRACE 
#define XSCP_M_ITTRACE(P_level, P_funcArea, trace_id)
#endif
#ifndef XTTL_M_RTTRACE
#define XTTL_M_RTTRACE(P_level, P_funcArea, trace_id)
#endif
#ifndef XSCP_M_RTTRACE
#define XSCP_M_RTTRACE(P_level, P_funcArea, trace_id)
#endif

/*********************************************************************************
 *	Following Macros are used by the Slee ONLY
 *
 *
 *
 *
 *********************************************************************************/
#ifndef XTEST_TR_FLAG
#define XTEST_TR_FLAG(P_level, P_funcArea)
#endif 
#ifndef TEST_TR_FLAG_TEST
#define TEST_TR_FLAG_TEST(P_level, P_funcArea) \
   { \
   int i ; \
   for (i=0; i < lgend; i++) \
       printf("\nCOM_isEnableLog[%02d]: %x", \
	      i, COM_isEnableLog[i]) ; \
   printf("\nCOM_isEnableLog[P_level]: %x\n P_level: %x\n P_funcArea: %x \
   \nCOM_isEnableLog[P_level] & P_funcArea: %x\n", \
   COM_isEnableLog[P_level], P_level, P_funcArea, \
   (COM_isEnableLog[P_level] & P_funcArea))  ; \
   } \
   if ( COM_isEnableLog[P_level] & P_funcArea ) 

#endif /* TEST_TR_FLAG_TEST */

#ifndef _WIN32
#define TTLX_FEATURE

/* Define the number of group that can be used. */
#define NGROUP 16

/* Array in which every thing is check. Declared elsewhere in libTTL */
extern COM_T_uint32    COM_isEnableExtLog[NGROUP+1][lgend+1];

/*******************************************************************/
/*  TTLX_M_TEST_TRACE_FLAG(P_level, P_group, P_funcArea)           */
/*                                                                 */
/*  DESCRIPTION:  A macro for testing the trace flags (deciding    */
/*                if a trace is to be outputed.                    */
/*                Refer to TTL_PRIV_M_TRACE_FLAG                   */
/*                                                                 */
/*  USES:         Internally by trace macros.                      */ 
/*                May be used externally                           */
/*                                                                 */
/*  PARAMETERS:                                                    */
/*                                                                 */
/*  P_group      The P_group passed with the macro.                */
/*                                                                 */
/*  P_level      The P_level passed with the macro.                */
/*                                                                 */
/*  P_funcArea  The P_funcArea passed with the macro.              */
/*                                                                 */
/*******************************************************************/
#ifndef TTLX_M_TEST_TRACE_FLAG
#define TTLX_M_TEST_TRACE_FLAG(P_level, P_group, P_funcArea) \
   if ( COM_isEnableExtLog[P_group][P_level] & P_funcArea ) 
#endif /* TTLX_M_TEST_TRACE_FLAG */



/*******************************************************************/
/*  TTLX_M_INIT(E_Name)                                            */
/*                                                                 */
/*                                                                 */
/*  DESCRIPTION: Initialize trace masks from debug.conf file       */
/*               Must be used in each main()                       */
/*               Refer to TTL_M_INIT                               */
/*                                                                 */
/*  USES:         Externally                                       */
/*                                                                 */
/*  PARAMETERS: E_Name: executable name                            */
/*                                                                 */
/*                                                                 */
/*******************************************************************/
#ifndef TTLX_M_INIT
#define TTLX_M_INIT(E_Name)    \
{                              \
  TTL_V_subsysId = TELECOM_SCP;\
  scp_m_init(E_Name);          \
  ttlx_m_init(E_Name);         \
}
/* To avoid to change all the main() */
#undef TTL_M_INIT
#define TTL_M_INIT TTLX_M_INIT
#undef SCP_M_INIT
#define SCP_M_INIT TTLX_M_INIT
#endif /* TTLX_M_INIT */

/*******************************************************************/
/*  TTLX_M_TRACE(P_level,P_group,P_funcArea,P_exp)                 */
/*                                                                 */
/*                                                                 */
/*  DESCRIPTION: call either printf functions or nettl to record   */
/*               a trace from source code.                         */
/*               Refer to TTL_M_TRACE                              */
/*                                                                 */
/*  USES:         Externally                                       */
/*                                                                 */
/*  PARAMETERS: P_group: Group of trace                            */
/*              P_level: trace level                               */
/*              P_funcArea: functional area                        */
/*              P_exp: expression to be printed or recorded        */
/*******************************************************************/
#ifndef TTLX_M_TRACE
#define TTLX_M_TRACE(P_level,P_group,P_funcArea,P_exp)  \
{ \
    TTLX_M_TEST_TRACE_FLAG(P_level, P_group, P_funcArea) \
    { \
	  char *L_str; \
	  L_str = TTL_sprintf P_exp; \
	  TTL_trace(P_level, P_funcArea, L_str, __LINE__, __FILE__); \
    } \
}
#endif /* TTLX_M_TRACE */

/*************************************************************************/
/*  TTLX_M_DTRACE(P_level,P_group,P_funcArea,P_str,P_typeId,P_dataPtr,   */
/*               P_dataLen)                                              */
/*                                                                       */
/*                                                                       */
/*  DESCRIPTION: call either printf functions or nettl to record         */
/*               a trace from source code. Allow data "dumping"          */
/*               Refer to TTL_M_DTRACE                                   */
/*                                                                       */
/*  USES:         Externally                                             */
/*                                                                       */
/*  PARAMETERS: P_group: Group of trace                                  */
/*              P_level: trace level                                     */
/*              P_funcArea: functional area                              */
/*              P_str: expression to be printed or recorded              */
/*              P_typeId: data type Id (to be dumped)                    */
/*              P_dataPtr: Ptr to the datat to be dumped                 */
/*              P_dataLen: length of the data to be dumped               */
/*************************************************************************/
#ifndef TTLX_M_DTRACE
#define TTLX_M_DTRACE(P_level,P_group,P_funcArea,P_str,P_typeId,P_dataPtr,P_dataLen)  \
{                                                                            \
  TTLX_M_TEST_TRACE_FLAG( P_level, P_group, P_funcArea) \
  { \
    TTL_dtrace(P_level, P_funcArea, P_str,P_typeId,P_dataPtr,P_dataLen, __LINE__, __FILE__); \
  } \
}
#endif /* TTLX_M_DTRACE */



/*******************************************************************/
/*  TTLX_M_NTRACE (P_level, P_group, P_funcArea, trace_id, P_exp)  */
/*                                                                 */
/*  DESCRIPTION:  An enhanced version of TTLX_M_TRACE allowing     */
/*                                                                 */
/*             	  b) a scaling factor allowing the user to pass  */
/*                   a parameter which is used to scale that       */
/*                   particular trace; that is, the trace is only  */
/*                   output once for the 'scale' number of times   */
/*                   the program passes this point.                */
/*                                                                 */
/*  PARAMETERS:                                                    */
/*                                                                 */
/*******************************************************************/
#ifndef TTLX_M_NTRACE
#define TTLX_M_NTRACE(P_level, P_group, P_funcArea, trace_id, P_exp) \
{  \
    TTLX_M_TEST_TRACE_FLAG(P_level, P_group, P_funcArea) \
    { \
        static int    trace_count = 0 ;   \
        static int    total_count = 0 ; \
        char *L_str; \
        L_str = TTL_sprintf P_exp; \
        TTL_ntrace(P_level, P_funcArea, trace_id, L_str, &trace_count, \
                   &total_count, __LINE__, __FILE__); \
    } \
}
#endif /* TTLX_M_NTRACE */

/*******************************************************************/
/*                                                                 */
/* MACRO: TTLX_M_NDTRACE(P_level, P_group, P_funcArea, trace_id,   */
/*                       descr, addr, length, element_size)        */
/*                                                                 */
/*  Parameters                                                     */
/*  ==========                                                     */
/*  P_group      as before                                         */
/*                                                                 */
/*  P_level      as before                                         */
/*                                                                 */
/*  P_funcArea   as before                                         */
/*                                                                 */
/*  Scale        A scaling factor defining how often a trace is to */
/*               be output.                                        */
/*                                                                 */
/*  Tag          See NTRACE                                        */
/*                                                                 */
/*  Descr        Description of what is being output               */
/*                                                                 */
/*  Addr         The address of the item to be dumped              */
/*                                                                 */
/*  Length       The size of the elements to be dumped             */
/*                                                                 */
/*  Element_size The number of elements to be dumped               */
/*                                                                 */
/*******************************************************************/
#ifndef TTLX_M_NDTRACE
#define TTLX_M_NDTRACE(P_level, P_group, P_funcArea, trace_id, descr, addr, length, element_size) \
{ \
    TTLX_M_TEST_TRACE_FLAG(P_level, P_group, P_funcArea) \
    { \
        static int    trace_count = 0 ;   \
        static int    total_count = 0 ; \
        char *L_str; \
        L_str = TTL_sprintf descr; \
        TTL_ndtrace(P_level, P_funcArea, trace_id, L_str, addr, length, element_size, \
					&trace_count, &total_count, __LINE__, __FILE__); \
    }\
}

#endif /* TTLX_M_NDTRACE */

/*******************************************************************/
/*                                                                 */
/* MACRO: TTLX_M_ITTRACE(P_level, P_group, P_funcArea, trace_id)   */
/*                                                                 */
/*  Description                                                    */
/*  ===========                                                    */
/*                                                                 */
/*     This trace initialises a static global variable (that is,   */
/*     visable)                                                    */
/*     within the source file where this inlude file included)     */
/*     with the current time. The TTLX_M_RTTRACE then read the     */
/*     timer and displays the total time and elapased timer        */
/*     relative to this start time.                                */
/*                                                                 */
/*  Parameters                                                     */
/*  ==========                                                     */
/*                                                                 */
/*     P_group, P_level, P_funcArea,      as for TTLX_M_TRACE      */
/*                                                                 */
/*     trace_id     used to configure the frequency of             */
/*                  initialisation of                              */
/*                  timers, to synchronize them with               */
/*                 for example) TTLX_M_NTRACE                      */
/*                                                                 */
/*******************************************************************/
#ifndef TTLX_M_ITTRACE
#define TTLX_M_ITTRACE(P_level, P_group, P_funcArea, trace_id) \
{  \
    TTLX_M_TEST_TRACE_FLAG(P_level, P_group, P_funcArea) \
    { \
        static int    trace_count = 0 ;   \
        static int    total_count = 0 ; \
        TTL_ittrace(P_level, P_funcArea, trace_id, &trace_count, &total_count, __LINE__, __FILE__); \
    } \
}
#endif /* TTLX_M_ITTRACE */

#ifndef TTLX_M_RTTRACE
#define TTLX_M_RTTRACE(P_level, P_group, P_funcArea, trace_id) \
{  \
    TTLX_M_TEST_TRACE_FLAG(P_level, P_group, P_funcArea) \
    { \
        static int    trace_count = 0 ;   \
        static int    total_count = 0 ; \
        TTL_rttrace(P_level, P_funcArea, trace_id, &trace_count, &total_count, __LINE__, __FILE__); \
    } \
}
#endif /* TTLX_M_RTTRACE */
/**********************************************************************
 *	End TTL Extension
 **********************************************************************/
#endif  /* _WIN32 */

/**********************************************************************
 *
 *		Function Declaration
 **********************************************************************/


#ifdef __cplusplus
extern "C" {
#endif

#if defined(__STDC__) || defined(__cplusplus)
#ifndef _WIN32
HPAIN_TTLAPI void ttlx_m_init(char *e_name); 
#endif /* _WIN32 */
HPAIN_TTLAPI void scp_m_init(char *e_name) ;
HPAIN_TTLAPI char *TTL_sprintf(char *P_fmt, ...);
HPAIN_TTLAPI void TTL_trace(int P_level, int P_funcArea, char *P_str, int P_line, char *P_file);
HPAIN_TTLAPI void TTL_dtrace(int P_level, int P_funcArea, char *P_str, int P_typeId, const void *P_dataPtr, int P_dataLen, int P_line, char *P_file);
HPAIN_TTLAPI void TTL_ntrace(int P_level, int P_funcArea, int P_trace_id, char *P_str, int *P_trace_count, int *P_total_count, int P_line, char *P_file);
HPAIN_TTLAPI void TTL_ndtrace(int P_level, int P_funcArea, int P_trace_id, char *P_str, const void *P_dataPtr, int P_dataLen, int P_element_size, int *P_trace_count, int *P_total_count, int P_line, char *P_file);
HPAIN_TTLAPI void TTL_ittrace(int P_level, int P_funcArea, int P_trace_id, int *P_trace_count, int *P_total_count, int P_line, char *P_file);
HPAIN_TTLAPI void TTL_rttrace(int P_level, int P_funcArea, int P_trace_id, int *P_trace_count, int *P_total_count, int P_line, char *P_file);
#define TTL_Alarm TTL_log
HPAIN_TTLAPI  void TTL_log(TTL_T_LogLevel P_level, TTL_T_threadControl P_threadType, int P_msgSetId, int P_msgId, ... );

#ifdef __cplusplus
}
#endif

#else	/* not __STDC__ nor C++ */

/* K&R declarations to cope with Message Set (compiled with acc DSET compiler) */
#ifndef _WIN32
HPAIN_TTLAPI void ttlx_m_init(); 
#endif /* _WIN32 */
HPAIN_TTLAPI void scp_m_init() ;
HPAIN_TTLAPI char *TTL_sprintf();
HPAIN_TTLAPI void TTL_trace();
HPAIN_TTLAPI void TTL_dtrace();
HPAIN_TTLAPI void TTL_ntrace();
HPAIN_TTLAPI void TTL_ndtrace();
HPAIN_TTLAPI void TTL_ittrace();
HPAIN_TTLAPI void TTL_rttrace();
HPAIN_TTLAPI  void TTL_log();

#endif /* __STDC__  or C++*/


#endif /* TTLCOMMON_H */


