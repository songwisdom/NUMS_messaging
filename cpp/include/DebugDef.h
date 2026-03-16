/*===============================================================================
*       MODULE NAME          : DebugDef.h
*       PURPOSE              : Expand Log.
*       INPUT ARGUMENT       : NONE
*       OUTPUT ARGUMENT      : NONE
*       RETURN VALUE         : NONE
*       REVISION             :     BY             WHEN            REASON
*                             -------------  --------------  -------------
*                                seoki        2001.11.24      copy cat 
==============================================================================*/

#ifndef __ELOG_H__
#define __ELOG_H__

#include <stdio.h>
#include <string.h>
#include "Debug.h"

#ifdef _HP_64BIT_SYS_TYPE_
#define PATH_MAX 1024
#endif

#ifdef __cplusplus
	extern "C" {
#endif
		/*
		int		MakeFileName(char*  Filename);
		void	LogTime(FILE *logFp);
		void	LogStart(char *cpHome, char *cpFile, char *cpID);
		void	LogQuit(const char *fmt, ...);
		void 	LogRet(const char *fmt, ...);
		void	LogStrDesc(u_char *cpDesc, u_char *cpData, int nSize, int nInt);
		void	LogStr(u_char *cpData, int nSize, int nInt);
		void	LogPrint(const char *fmt, ...);
		*/
#ifdef __cplusplus
	}
#endif

///////// for Debugging
//////
///
#define	TERM_PTR				printf
#define	FILE_PTR				LogRet

#ifndef DLOG

#if (_DLEVEL>=3)
#	define LINE_PTR				TERM_PTR
#	define DPRT(arg)			LINE_PTR arg
#	define DLOG(arg)			LINE_PTR arg
#	define VALPTR(type, argv)	valprt_##type(argv)

#elif (_DLEVEL==2)
#	define LINE_PTR				FILE_PTR
#	define DPRT(arg)			TERM_PTR arg
#	define DLOG(arg)			LINE_PTR arg
#	define VALPTR(type, argv)   valprt_##type(argv)

#elif (_DLEVEL==1)
#	define LINE_PTR				FILE_PTR
#	define DPRT(arg)			TERM_PTR arg
#	define DLOG(arg)			LINE_PTR arg
#	define VALPTR(type, argv)

#else
#	define LINE_PTR
#	define DPRT(arg)
#	define DLOG(arg)
#	define VALPTR(type, argv)

#endif

#endif //DLOG

// built-in type
// For USMSC New Type Date 09.04.08
#define valprt_char(arg) valprt_char(arg)
#define valprt_short(arg) valprt_short(arg)
#define valprt_int(arg) valprt_int(arg)

#define valprt_u_char(arg) valprt_u_char(arg)
#define valprt_u_short(arg) valprt_u_short(arg)
#define valprt_u_int(arg) valprt_int(arg)

//#define valprt_int(arg) LINE_PTR(#arg " = %-10i [0x%08X]\n", arg, arg)
#define valprt_int(arg)		LINE_PTR("%20s = %10i [0x%08x] P[%s:%i]\n", \
								#arg, arg, arg, __FILE__, __LINE__)

#define valprt_int(arg)	LINE_PTR("%20s  = %10u [0x%08x] P[%s:%i]\n", \
								#arg, arg, arg, __FILE__, __LINE__)

#define valprt_long(arg)	valprt_int(arg)

#define valprt_u_long(arg)	valprt_int(arg)

#define valprt_short(arg)	LINE_PTR("%20s = %5i [0x%04x] P[%s:%i]\n", \
								#arg, arg, arg, __FILE__, __LINE__)

#define valprt_u_short(arg)	LINE_PTR("%20s = %5u [0x%04x] P[%s:%i]\n", \
								#arg, arg, arg, __FILE__, __LINE__)

#define valprt_char(arg)	LINE_PTR("%20s = '%c'(%3i) [0x%02x] P[%s:%i]\n", \
								#arg, (char)arg, arg, arg, __FILE__, __LINE__)
#define valprt_u_char(arg)	LINE_PTR("%20s = '%c'(%3u) [0x%02x] P[%s:%i]\n", \
								#arg, (char)arg, arg, arg, __FILE__, __LINE__)

// user define type
#define valprt_pointer(arg)	LINE_PTR("%20s = ADDR[0x%p] P[%s:%i]\n", \
								#arg, arg, __FILE__, __LINE__)
#define valprt_string(arg)	LINE_PTR("%20s = [%s] P[%s:%i]\n", \
								#arg, arg, __FILE__, __LINE__)

#define valprt_array(arg) \
				{	LINE_PTR(#arg " = %s\n", arg); \
					string strbuf = (string) malloc(strlen(arg)*4+1);\
					string pstr, idx; register loop = 0; char spt = '\0';\
					for (pstr = strbuf, idx = arg; *idx; idx++, pstr += 4) {\
						spt = (++loop % 20) ? '\0' : '\n';\
						sprintf(pstr, "[%02X]%c", *idx & 0xff, spt);\
						pstr += (spt) ? 1 : 0;\
					}\
					LINE_PTR("%s%c", strbuf,(spt) ? '\0': '\n'); free(strbuf);\
				}

#ifdef _DLEVEL

#	define ERRPOS() \
				DLOG(("ERROR [0x%04X :%s] %s()- %i in %s\n", errno, \
						strerror(errno), __func__, __LINE__, __FILE__))

#	define ERRVPOS(type, arg) \
			{	VALPTR(type, arg); \
				DLOG(("%s ERROR [0x%04X :%s] %s()- %i in %s\n", #arg, errno, \
						strerror(errno), __func__, __LINE__, __FILE__)); \
			}

#else// ! _DLEVEL

#	define ERRPOS() \
				LogRet("ERROR [0x%04X :%s] %s()- %i in %s\n", errno, \
						strerror(errno), __func__, __LINE__, __FILE__)

#	define ERRVPOS(type, arg) \
			{	VALPTR(type, arg); \
				LogRet("%s ERROR [0x%04X :%s] %s()- %i in %s\n", #arg, errno, \
						strerror(errno), __func__, __LINE__, __FILE__); \
			}

#endif // end of  _DLEVEL

#endif // __ELOG_H__
