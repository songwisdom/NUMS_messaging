#ifndef     _DEBUG_H_
#define     _DEBUG_H_ 

#include "FLKSimBaseDef.h"
#include "flk_type.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "DebugDef.h"

//***********************************************************************/
/*      Debug  Function Macro                                          */
/***********************************************************************/
#define DEBUGON     1
#define DEBUGOFF   -1
#define DEBUGFLAG   001

#define CMT_LENG			108
#define ALARMRCD_SIZE		128
#define MAX_FILE_LENG		90
#define MAX_ALARMDB_RECNUM	500
#define MAX_TRACE_MIN		5
#define MAX_DEBUG_NUM		50
//#define SIM_PATH_MAX 		512


typedef enum {
	DG_LEVEL_ZERO,	/* INPUT/OUTPUT */
	DG_LEVEL_ONE,	/* INTERNAL     */
	DG_LEVEL_TWO,	/* COMT or OTHER */
	DG_LEVEL_THREE,	/* COMT or OTHER */
	DG_LEVEL_FOUR,	/* COMT or OTHER */
	DG_LEVEL_FIVE	/* COMT or OTHER */
} eDebugLevel_t;

typedef enum {
	DG_NO_CLEAR,
	DG_CLEAR
} eDebugClear_t;


/***********************************************************************/
/*      Alarm  Function Macro                                          */
/***********************************************************************/

#if 0
typedef enum {
	ALARM_OFF,
	ALARM_ON
} eAlarmFlag_t;
#endif

typedef struct
{
    short 	level;
    pid_t 	pid;
    u_char	min[DOUBLE_SIZE];
    char	f_name[SIM_PATH_MAX];
} stTraceData;

typedef struct
{
    stTraceData	Data[MAX_DEBUG_NUM];
} stDebug;

typedef struct
{
    u_short		Level;
    stTraceData	Data[MAX_TRACE_MIN];
} stTrace;
 
typedef struct
{
    u_int 	AlarmCode;
    u_int  SubCode;
    u_int 	STime;
    u_int 	status;
   	u_int 	Count;
   	u_int 	Flag;
    char	Comment[CMT_LENG];
}stAlarmDBTable;

extern  stDebug*	DebugData;

//extern  u_int 		DebugAddr;
extern  key_t 		DebugAddr;

extern  u_short 		DebugIndex;

extern  stTrace*    TraceData;
extern  u_int 		TraceAddr;
extern  int         TraceIndex;

extern  char        AlarmFile[MAX_FILE_LENG];

#ifdef __cplusplus
extern "C" {
#endif
int MakeFileName(char*	Filename);
void LogStart(char *cpHome, char	*cpFile, char *cpID);
void LogTime(FILE *Logfp);
void LogQuit(const char *fmt, ...);
void LogRet(const char *fmt, ...);
void LogStrDesc(u_char *cpDesc, u_char *cpData, int nSize, int nInt);
void LogStr(u_char *cpData, int nSize, int nInt);
void LogPrint(const char *fmt, ...);

#ifdef __cplusplus
}
#endif
#endif


