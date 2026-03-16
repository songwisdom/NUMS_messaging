
/*******************************************************************************
**                                                                   		  **
**     Copyright (c) 1996 LG Information & Communications                  	  **
**     All rights reserved.                                                   **
**                                                                            **
**     This program contains confidential and proprietary information of      **
**     LGIC, and any reproduction, disclosure, or use in whole or in part     **
**     is expressly  prohibited, except as may be specifically authorized     **
**     by prior written agreement or permission of LGIC.                      **
**                                                                            **
**                   LG Information & Communications, Ltd.                    **
**            60-39, Kasan-dong, Keumchun-ku, Seoul 153-023 Korea             **
**                                                                            **
*******************************************************************************/
/**
 @ingroup Debug
 @{ 
 */

/*******************************************************************************
	FILE NAME : nnlog.c
	PURPOS : Logging routines.
 		 The functions in this file are independent of any application
		 variables, and may be used with any C program.

	DATE OF WRITE :
	AUTHOR :		(LGIC) KIHYUN KIM 
	REVISION;
		BY         WHEN     REASON
		---------- -------- ----------------------------------------
		KHKIM      97. 4. 1  First Coding

*******************************************************************************/



#include    <stdio.h>
#include    <stdarg.h>
#include    <sys/types.h>
#include    <sys/ipc.h>
#include    <sys/sem.h>
#include    <time.h>
#include    <sys/fcntl.h>
#include    <sys/errno.h>
#include    <sys/stat.h>
#include    <errno.h>
#include    <sys/times.h>
#include    <unistd.h>
#include    <stdlib.h>
#include 	"Debug.h"

#define		DATE_TIME_FORM	"%Y.%m.%d %H:%M:%S"

char	cpLogFile[128];
FILE*	logfd;

static FILE	* OpenLogFile(char *cpLFile);

int
MakeFileName(char*	Filename)
{
	time_t	tTime;
	struct tm	*spTm;
	char	cpDate[11];

	tTime = time((time_t *)NULL);

	spTm = localtime(&tTime);
	memset(&cpDate, 0x00, sizeof(cpDate));
	if (spTm->tm_year >= 90)
		strftime(cpDate, 11, "19%y%m%d", spTm);
	else
		strftime(cpDate, 11, "20%y%m%d", spTm);

	sprintf(Filename, "%s.%s", Filename, cpDate);
	return 1;
}

static FILE	* OpenLogFile(char *cpLFile)
{

	time_t tTime = time((time_t *)NULL);

	struct tm *spTm = localtime(&tTime);

	char	cpDate[11];
	memset(&cpDate, 0x00, sizeof(cpDate));

	if(spTm->tm_year >= 100)
		strftime(cpDate, 11, "20%y%m%d", spTm);
	else 
		strftime(cpDate, 11, "19%y%m%d", spTm);

	char *cpHomeDir = getenv("SMSC_HOME");

	if (NULL == cpHomeDir) return NULL;

	if((strlen(cpLFile) <= strlen(cpHomeDir)) && 
		strncmp(cpLFile, cpHomeDir, strlen(cpHomeDir)))
	{
		sprintf(cpLFile,"%s/tmp/pid%06u", cpHomeDir, getpid());
	}

	char cpFile[256];
	bzero(cpFile, sizeof(cpFile));
	sprintf(cpFile, "%s.%s", cpLFile, cpDate);

	return(fopen(cpFile, "a+"));
}

/*------------------------------------------------------------------------------
 *
 *	FUNCTION NAME : LogStart()
 *	PURPOSE : Logging file name setup function.
 *	INPUT ARGUMENTS : LogStart(cpHome, cpFile, cpID)
 *	RETURN VALUE :	void
 *
 *	CALLED FROM :
 *	USED GLOBAL VARIABLES :
 *		1) EXTERNAL
 *		2) LOCAL
 *
 *	REVISION;
 *		BY         WHEN     REASON
 *		---------- -------- --------------------------------------
 *      KHKIM      97. 4. 1  First Coding
 * 
------------------------------------------------------------------------------*/
void
LogStart(char *cpHome, char	*cpFile, char *cpID)
{
	if((cpID == (char *)NULL) || (*cpID == '\0'))
		sprintf(cpLogFile, "%s%s_log", cpHome, cpFile);
	else
		sprintf(cpLogFile, "%s%s/%s", cpHome, cpFile, cpID);

	return;
}

/*------------------------------------------------------------------------------
 *
 *	FUNCTION NAME : LogTime()
 *	PURPOS : Logging Current Time.
 *	INPUT ARGUMENTS : LogTime(Logfp)
 *	RETURN VALUE :	void
 *
 *	CALLED FROM :
 *	USED GLOBAL VARIABLES :
 *		1) EXTERNAL
 *		2) LOCAL
 *
 *	REVISION;
 *		BY         WHEN     REASON
 *		---------- -------- --------------------------------------
 *      KHKIM      97. 4. 1  First Coding
 * 
------------------------------------------------------------------------------*/
void
LogTime(FILE *Logfp)
{
	time_t	tTime;
	struct tm	*spTm;
	char	cpDate[30];

	tTime = time((time_t)NULL);
	spTm = localtime(&tTime);

	memset(cpDate, 0x00, sizeof(cpDate));
	strftime(cpDate, sizeof(cpDate), DATE_TIME_FORM, spTm);
	fprintf(Logfp, "%s ", cpDate);
	return;
}

/*------------------------------------------------------------------------------
 *
 *	FUNCTION NAME : LogQuit()
 *	PURPOS : Fatal error.  Logging a message and terminate.
 *		 	 Don't dump core.
 *	INPUT ARGUMENTS : LogQuit(str, arg1, arg2, ...)
 * 			  The string "str" must specify the conversion
 *			  specification for any args.
 *	RETURN VALUE :	void
 *
 *	CALLED FROM :
 *	USED GLOBAL VARIABLES :
 *		1) EXTERNAL
 *		2) LOCAL
 *
 *	REVISION;
 *		BY         WHEN     REASON
 *		---------- -------- --------------------------------------
 *      KHKIM      97. 4. 1  First Coding
 * 
------------------------------------------------------------------------------*/
void
LogQuit(const char *fmt, ...)
{
	FILE	*Logfp;
	va_list args;

	va_start(args,fmt);
	if((Logfp = OpenLogFile(cpLogFile)) == (FILE *)NULL)
	{
		LogTime(stderr);
		vfprintf(stderr, fmt, args);
		fputc('\n', stderr);
	}
	else
	{
		LogTime(Logfp);
		vfprintf(Logfp, fmt, args);
		fclose(Logfp);
	}

	va_end(args);
	return;
}

/*------------------------------------------------------------------------------
 *
 *	FUNCTION NAME : LogRet()
 *	PURPOS : Logging a message, and return to caller.
 *	INPUT ARGUMENTS : LogRet(str, arg1, arg2, ...)
 * 			  The string "str" must specify the conversion
 *			  specification for any args.
 *	RETURN VALUE :	void
 *
 *	CALLED FROM :
 *	USED GLOBAL VARIABLES :
 *		1) EXTERNAL
 *		2) LOCAL
 *
 *	REVISION;
 *		BY         WHEN     REASON
 *		---------- -------- --------------------------------------
 *      KHKIM      97. 4. 1  First Coding
 * 
------------------------------------------------------------------------------*/
void
LogRet(const char *fmt, ...)
{
	va_list args;
	FILE    *Logfp;

	va_start(args,fmt);

	if (NULL == (Logfp = OpenLogFile(cpLogFile)))
	{
		LogTime(stderr);
		vfprintf(stderr, fmt, args);
		fputc('\n', stderr);
		fflush(stderr);
	}
	else {
		LogTime(Logfp);
		vfprintf(Logfp, fmt, args);
		if (fmt[strlen(fmt) - 1] != '\n') fputc('\n', Logfp);
		fclose(Logfp);
	}
	va_end(args);
	return;
}

void
LogStrDesc(u_char *cpDesc, u_char *cpData, int nSize, int nInt)
{
    FILE    *Logfp;

    if((Logfp = OpenLogFile(cpLogFile)) == (FILE *)NULL)
        return;
	fprintf(Logfp, "%s : ", cpDesc); 

	int nIdx;
	for(nIdx=0; nIdx<nSize; nIdx++)
	{
		if((nIdx!=0) && ((nIdx%nInt)==0))
			fprintf(Logfp, "\n");

		fprintf(Logfp, "%02x ", cpData[nIdx]);
	}

	fprintf(Logfp, "\n");
	fclose(Logfp);
	return;
}

void
LogStr(u_char *cpData, int nSize, int nInt)
{
    FILE    *Logfp;

    if((Logfp = OpenLogFile(cpLogFile)) == (FILE *)NULL)
        return;

	int nIdx;
	for(nIdx=0; nIdx<nSize; nIdx++)
	{

		if((nIdx!=0) && ((nIdx%nInt)==0))
			fprintf(Logfp, "\n");

		fprintf(Logfp, "%02x ", cpData[nIdx]);
	}

	fprintf(Logfp, "\n");
	fclose(Logfp);
	return;
}

void
LogPrint(const char *fmt, ...)
{
	va_list		args;
	FILE		*Logfp;

	va_start(args,fmt);
	if((Logfp = OpenLogFile(cpLogFile)) == (FILE *)NULL)
	{
		vfprintf(stderr, fmt, args);
		fputc('\n', stderr);
		fflush(stderr);
	}
	else {
		vfprintf(Logfp, fmt, args);
		fputc('\n', Logfp);
		fclose(Logfp);
	}

	va_end(args);
}
/** @} */
