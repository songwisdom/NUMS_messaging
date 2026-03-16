/***********************************************************
*
* $Header: /home/cvs/smsc/skt-smsc/include/ksig7/ainCommon.h,v 1.1 2008/07/21 05:12:47 prowoni Exp $
*
* $Source: /home/cvs/smsc/skt-smsc/include/ksig7/ainCommon.h,v $
* $Revision: 1.1 $
* $Locker:  $
*
***********************************************************/
/* basic definitions of types and manifest constants */

/**************************************************************************
*  Information about using this file:
*
*    It should be included in your file via #include <ainCommon.h>
*
*    Before including it in the file that implements the main()
*    function, you should #define EXEC_MAIN in order to properly
*    instanciate the variables needed for tracing.
*
*    It can be included in any other file (library or otherwise)
*    without defining EXEC_MAIN in order to have the externs and the
*    macro definitions...
*
**************************************************************************/

#ifndef AINCOMMON_H
#define AINCOMMON_H

#ifndef FLAVER_WHAT_STRING_
#define FLAVER_WHAT_STRING_
#ifdef CM_FLAVOR_WHAT_STRING
static const char FlavorWhatString[] = "@(@) "CM_FLAVOR_WHAT_STRING ;
#else
static const char FlavorWhatString[] = "@(@) <NO DEFINED FLAVOR>" ;
#endif 
#endif

#ifdef _WIN32
#include <windows.h>
#else
#ifndef _HPUX_SOURCE
#define _HPUX_SOURCE
#endif /* _HPUX_SOURCE */
#include <sys/time.h>
//#include <sys/stdsyms.h>
#endif


#include <time.h>
#include <limits.h>
#include <stdlib.h>

/* Introduced for 40k socket support */
#ifdef _USE_BIG_FDS
#  define INIT_BIG_FD_MODE() MSG_SetBigFdMode (_MAXFUPLIM, sizeof (fd_set))
#endif
#define FD_ZERO_SMALL(p)   memset ((void *)(p), (int) 0, MSG_GetFdSetSize ())
#define MAX_CHANNELS       2048

#define MAXLEN_FILENAME    _POSIX_NAME_MAX
#ifndef _WIN32
#define MAXLEN_FILEPATH    _POSIX_PATH_MAX
#else
#define MAXLEN_FILEPATH    512
#endif
#define MAXLEN_HOSTNAME    20
#define MAXLEN_SERVICENAME 20

#ifndef true
#define true 1
#define false 0
#endif

#ifndef TRUE
#define TRUE 1
#define FALSE 0
#endif

#ifndef NULL
#define NULL        ((void *)0)
#endif /* NULL */

/**************************************************************************
* Some typedefs for various common data types...
**************************************************************************/
typedef int		         COM_T_int;
typedef unsigned char    COM_T_uint8;
typedef unsigned short   COM_T_uint16;

#ifdef __LP64__
typedef unsigned int    COM_T_uint32;
#else
typedef unsigned long    COM_T_uint32;
#endif /* __LP64__ */

typedef unsigned int     COM_T_boolean;
typedef unsigned char    COM_T_byte;
typedef double           COM_T_real;

#ifdef __LP64__
typedef struct tag_COM_T_timeStamp { 
        COM_T_uint32  tv_sec;         /* seconds */
        COM_T_int    tv_usec;        /* and microseconds */
        } COM_T_timeStamp;
#else
typedef struct timeval   COM_T_timeStamp;
#endif /* __LP64__ */

#ifdef __LP64__
typedef unsigned int     COM_T_dateStamp;
#else
typedef unsigned long    COM_T_dateStamp;
#endif /* __LP64__ */

typedef int              COM_T_union;
    /* Followed by type data, length dependant on actual union */
typedef unsigned char    COM_T_lenString;
typedef unsigned char    COM_T_string;
    /* Followed by string data, length dependant on actual string */
typedef unsigned char    COM_T_lenBcd;
typedef unsigned char    COM_T_bcd;
    /* Followed by bcd data, length dependant on actual bcd */

typedef struct {
  COM_T_byte len;
  COM_T_byte subIdBytes[31];
} COM_T_objectId;

typedef struct {
  COM_T_objectId	oid;
  COM_T_byte		mask[4];
} COM_T_maskedObjectId;

#ifdef __LP64__
typedef struct timeval_32 {
        COM_T_uint32  tv_sec;         /* seconds */
        COM_T_int    tv_usec;        /* and microseconds */
        } timeval_32;
#endif /* __LP64__ */

/* Note all TB_T_types should now be obtained by #include'ing TB_basic.h */

#include <ttlCommon.h>

#endif /* AINCOMMON_H */
