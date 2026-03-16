/***********************************************************
*
* $Header: /home/cvs/smsc/skt-smsc/include/ksig7/TimerLib.h,v 1.1 2008/07/21 05:12:46 prowoni Exp $
*
* $Source: /home/cvs/smsc/skt-smsc/include/ksig7/TimerLib.h,v $
* $Revision: 1.1 $
* $Locker:  $
* 
***********************************************************/
/*
// Filename: 
// Creation date: 
// Creator: BW
// Purpose:	
*/

/* 
 * @HB : Changed for Windows NT. Changes made by #if(n)def COCA_WIN32.
 *       Eliminated use of SIGALM, which NT does not provide.
 *       Most of the changes just to make this a Windows NT DLL.
 *                     Henk B Feb '98
 */

#ifndef TIMERLIB_H
#define TIMERLIB_H

/* Required files */
#include <ainCommon.h>

#ifdef COCA_WIN32

#  include <windows.h>
#  include <CocaUx4Nt.h>
#endif /* COCA_WIN32 */
/* Machine parameters */
#include <machine/param.h>

#ifndef _BIG_ENDIAN
#error Must_be_on_a_big_endian_architecture
#endif

/* Macro definition for UX11 64 */
#ifdef COMPAT3264
/* Assumes the seconds and microseconds are not longer than 'int' 
 * In the union, we fill the whole memory space and we assign only the 
 * higher integer part to the destination variable */
#define Long_in_Integer(dest,src) \
            {union {int  iSmall[2] /* 2 int to have the same memory space as a long */; \
                    long lBig /* The long variable */;} int_long; \
             int_long.lBig = (src); \
             (dest) = (COM_T_uint32)(int_long.iSmall[1]);}

#define COMfromtval(dest,src)   \
            {Long_in_Integer( (dest).tv_sec , (src).tv_sec ); \
             Long_in_Integer( (dest).tv_usec , (src).tv_usec );}
#else
#define LongInInteger(dest,src) \
             (dest)=(src)
#define COMfromtval(dest,src)  \
             (dest)=(src) 
#endif 

#ifdef COMPAT3264
#define tvalfromCOM(dest,src)  \
            {(dest).tv_sec=(unsigned long)(src).tv_sec; \
             (dest).tv_usec=(long)(src).tv_usec;}
#else 
#define tvalfromCOM(dest,src)  \
             (dest)=(src) 
#endif 

/* END Macro definition for UX11 64 */


#ifdef __cplusplus
extern "C" {
#endif

/* Defines */
#ifdef _WIN32
#  ifndef HPAIN_TIMERAPI
#      define HPAIN_TIMERAPI __declspec(dllimport)
#  endif
#else
#  define HPAIN_TIMERAPI 
#endif



#define TIMER_ONCE	1
#define TIMER_REPEAT	2

#define TIMER_ABSOLUTE	false
#define TIMER_RELATIVE	true

/* extern exported values */
HPAIN_TIMERAPI extern int TIMERXX_UseCR16;

/* Types and structures */

/* callback function */
typedef void (*TIMER_T_CB)(void *, void *, COM_T_timeStamp);
#ifndef COCA_WIN32
typedef void (*TIMER_T_SIGCB)(int, int, void *);
#endif

/* API functions */

/* Init and destroy */
HPAIN_TIMERAPI int TIMER_init_module(int nTimers);
HPAIN_TIMERAPI int TIMER_destroy_module(int tId);

#ifdef COCA_WIN32
/* return smallest timeout required for WFMO call */
HPAIN_TIMERAPI void ntTIMER_select_time(int *time);
#endif
/* takes a pointer to a pointer to allow it to be set to NULL, for select(2) when NO timeout */
HPAIN_TIMERAPI void TIMER_select_time(struct timeval **t); /* UX11 64 , CHECKED, realy a timeval, because use for external select */	
HPAIN_TIMERAPI void TIMER_handler(int n);

HPAIN_TIMERAPI void *TIMER_set_timer(COM_T_timeStamp ts, TIMER_T_CB cb, void *v, void *p, int del, int rel);
HPAIN_TIMERAPI void *TIMER_set_timer_t(int /* UX11 64 */ s, int /* UX11 64 */ u, TIMER_T_CB cb, void *v, void *p, int del, int rel);
HPAIN_TIMERAPI void TIMER_cancel_timer(void *tp);

#ifndef COCA_WIN32
/* For using sigalm, either with timerlib in control or user in control */
void TIMER_use_sigalm(TIMER_T_SIGCB users_sig_cb, COM_T_timeStamp res);
void TIMER_set_timeofday_var(COM_T_timeStamp *theirtimep);
#endif

/* also export timestamp handling functions */
HPAIN_TIMERAPI COM_T_timeStamp TIMER_ts_now(void);	
HPAIN_TIMERAPI struct timeval   TIMER_ts_now_Real(void);/* UX11 64 */
HPAIN_TIMERAPI int TIMER_ts_lessthan(COM_T_timeStamp t1, COM_T_timeStamp t2);
HPAIN_TIMERAPI int TIMER_ts_lessthan_tv(COM_T_timeStamp t1, struct timeval t2); /* UX11 64 New */
HPAIN_TIMERAPI int TIMER_ts_equals(COM_T_timeStamp t1, COM_T_timeStamp t2);
HPAIN_TIMERAPI COM_T_timeStamp TIMER_ts_sub(COM_T_timeStamp t1, COM_T_timeStamp t2);
HPAIN_TIMERAPI COM_T_timeStamp TIMER_ts_add(COM_T_timeStamp t1, COM_T_timeStamp t2);


HPAIN_TIMERAPI extern const COM_T_timeStamp TIMER_ts_never;	/* constant for the "never" time */

/* Error returns */


#ifdef __cplusplus
}	/* extern "C" */
#endif

#endif  /* TIMERLIB_H */



