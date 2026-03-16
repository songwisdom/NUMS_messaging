/**
 * =====================================================================================
 *
 *       @file  SimLog.c
 *
 *    	 @brief 
 *
 *       Version:  1.0
 *       Compiler:  cc (HP-UX)
 *       @date:  04/28/09 16:29:22
 *
 *       @author:  SMS Dev (), sms_dev@feelingk.com
 *       Company:  (c) FEELingK
 *       @attention
 *       Copyright (c) 2003 FEELINGK Co., Ltd.                                
 *
 *         All rights reserved.                                                
 *                                                                          
 *     This program contains confidential and proprietary information of    
 *     FEELINGK, and any reproduction, disclosure, or use in whole or in part 
 *     by prior written agreement or permission of FEELINGK.                 
 *                                                                          
 *                            FEELINGK Co., Ltd.                           
 *     3F Ace Techno Twin Tower 1, 212-1 Kuro-Dong, Kuro-Gu, Seoul 152-050 Korea 
 *
 * =====================================================================================
 */

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdarg.h>
#include "flk_type.h"
//#include "flk_string.h"
#include <time.h>
#include <sys/time.h>
#include "flk_sim_log.h"

static log_st stLog;

/*
static char *LogStatus[] = {
	"OFF",
	"ERR",
	"SUCCESS",
	"WARN",
	"NORMAL",
	"MESSAGE",
	"PARAM",
	"DEBUG",
	"ALL"
};

typedef struct log_t 
{
	char 		DispLevel;
	char 		FileLevel;
	char 		LogFile[SIM_LOG_FILE_SIZE];
	char 		*PushBuffer[100];
	int 		PushArgs;
}log_st;
*/

void log_do_write_display ( u_char DispLevel, char *Str)
{
	char *Color = NULL;

#if 0
	switch ((DispLevel << 4)) {
		case DISP_LEV_ERR :
			Color = SETCOLOR_FAILURE; break;
		case DISP_LEV_SUCCESS :
			Color = SETCOLOR_SUCCESS; break;
		case DISP_LEV_WARN :
			Color = SETCOLOR_WARNING; break;
		default :
			Color = SETCOLOR_NORMAL; break;
	}

	fprintf(stdout, "%s%s%s",Color, Str, SETCOLOR_NORMAL);
#endif
	return;
}

void log_do_write_file ( char *cpLogFile, char *Str)
{
	char log_file[1024] = {0x0, };
	FILE *fp = NULL;
	struct tm *tm_buf;
	struct timeval  tv;

	gettimeofday(&tv, NULL);
	tm_buf = localtime(&tv.tv_sec);

	sprintf(log_file, "%s.%02d%02x%02d", cpLogFile, (tm_buf->tm_year - 100 ), tm_buf->tm_mon +1, tm_buf->tm_mday);

	fp = fopen(log_file, "a+");
	if ( fp == NULL ) {
		log_do_write_display(DISP_LEV_ERR, "LogFile fopen failed");
		stLog.FileLevel = FILE_LEV_OFF;
		return;
	}

	fprintf( fp, "%s", (char *)Str);
	fclose(fp);
	return;
}
void log_write( u_char Lev, const char *fmt, ...) 
{
	char str[2048] = {0x0, };
	va_list	args;
	char DispLevel = GET_DISPLAY_LEVEL(Lev);
	char FileLevel = GET_FILE_LEVEL(Lev);
	va_start(args,fmt);
	vsprintf(str,fmt,args);
	va_end(args);

	if ( Lev != DISP_LEV_OFF && stLog.DispLevel != DISP_LEV_OFF && DispLevel <= stLog.DispLevel)
		log_do_write_display(DispLevel, (char *)str);
	if ( Lev != FILE_LEV_OFF && stLog.FileLevel != FILE_LEV_OFF && FileLevel <= stLog.DispLevel)
		log_do_write_file(stLog.LogFile, (char *)str);
	return;
}

static char hex_ascii[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
void log_do_write_displayHex ( u_char DispLevel, char *Comment, char *Data, int size, int SortFlag)
{
	char *Color = NULL;
	int idx = 0;
	
#if 0
	switch (DispLevel) {
		case DISP_LEV_ERR :
			Color = SETCOLOR_FAILURE; break;
		case DISP_LEV_SUCCESS :
			Color = SETCOLOR_SUCCESS; break;
		case DISP_LEV_WARN :
			Color = SETCOLOR_WARNING; break;
		default :
			Color = SETCOLOR_NORMAL; break;
	}

	fprintf(stdout, "%s%s",Color, Comment);
	for ( idx = 0; idx < size; ++idx ) {
		if (SortFlag > 0 && idx != 0 && idx % SortFlag == 0 )
			fprintf(stdout, "\n");
		fprintf(stdout, "[%c%c]", hex_ascii[ ((Data[idx] & 0xF0)>>4) ], hex_ascii[ (Data[idx] & 0x0F)]);
	}
	fprintf(stdout, "%s\n", SETCOLOR_NORMAL);
#endif
	return;
}
void log_do_write_fileHex ( char *cpLogFile, char *Comment, char *Data, int size, int SortFlag)
{
	char log_file[1024] = {0x0, };
	FILE *fp = NULL;
	struct tm *tm_buf;
	struct timeval  tv;
	int idx =0;

	gettimeofday(&tv, NULL);
	tm_buf = localtime(&tv.tv_sec);

	sprintf(log_file, "%s.%02d%02x%02d", cpLogFile, (tm_buf->tm_year - 100), tm_buf->tm_mon +1, tm_buf->tm_mday);

	fp = fopen(log_file, "a+");
	if ( fp == NULL ) {
		log_do_write_display(DISP_LEV_ERR, "LogFile fopen failed");
		stLog.FileLevel = FILE_LEV_OFF;
		return;
	}

	fprintf( fp, "%s", Comment);
	for ( idx = 0; idx < size; ++idx ) {
		if (idx != 0 && idx % SortFlag == 0 )
			fprintf(fp, "\n");
		fprintf(fp, "[%02X]", Data[idx]);
	}

	fclose(fp);
	return;
}

void log_writeHex( u_char Lev, char *Comment, char *Data, int nLen, int SortFlag )
{
	char DispLevel = GET_DISPLAY_LEVEL(Lev);

	if ( SortFlag < 0 )
		SortFlag = 0;

	if ( Lev != DISP_LEV_OFF && stLog.DispLevel != DISP_LEV_OFF && DispLevel <= stLog.DispLevel)
		log_do_write_displayHex(Lev, Comment, Data, nLen, SortFlag);
	/*
	if ( Lev != FILE_LEV_OFF && stLog.FileLevel != FILE_LEV_OFF && FileLevel <= stLog.FileLevel)
		log_do_write_fileHex(stLog.LogFile, Comment, Data, nLen, SortFlag);
		*/

	return;
}

void log_popup ( )
{
	int idx = 0;
	char *Ptr = NULL;
	char Level = 0;
	for (idx =0; idx < stLog.PushArgs; ++idx) {
		Ptr = stLog.PushBuffer[idx];
		Level = GET_DISPLAY_LEVEL(Ptr[0]);
		if ( Level != DISP_LEV_OFF && stLog.DispLevel != DISP_LEV_OFF && Level <= stLog.DispLevel) {
			puts("start display");
			log_do_write_display(Level, &Ptr[1]);
		}
		Level = GET_FILE_LEVEL(Ptr[0]);
		printf("log level (%d)\n", Level);
		if ( Level != FILE_LEV_OFF && stLog.FileLevel != FILE_LEV_OFF && Level <= stLog.FileLevel) {
			log_do_write_file(stLog.LogFile, &Ptr[1]);
			puts("start log");
		}
		free(Ptr);
	}
	stLog.PushArgs = 0;
	return;
}
void log_push ( u_char Lev, const char *fmt, ...)
{
	char str[2048] = {0x0, };
	char *Ptr = NULL;
	va_list	args;
	int Size = 0;
	va_start(args,fmt);
	vsprintf(str,fmt,args);
	va_end(args);

	if ( stLog.PushArgs == SIM_LOG_MAX_ARGS )
		log_popup();

	Size = (int)strlen (str);
	stLog.PushBuffer[stLog.PushArgs] = (char *)malloc(Size +2);
	Ptr = stLog.PushBuffer[stLog.PushArgs];
	if ( Ptr == NULL )
		return;

	bzero(Ptr, (Size+2));
	Ptr[0] = Lev;
	strcpy ((char *)Ptr +1, str);
	//*stLog.PushBuffer[stLog.PushArgs] = &Ptr;
	stLog.PushArgs++; 
	return;
}

void setSimLogLevelReset ( u_char DispLev, u_char FileLev)
{
		//stLog.DispLevel = GET_DISPLAY_LEVEL(DispLev);
	if ( (DispLev & 0xF0) != 0x00 )
		stLog.DispLevel = (DispLev >> 4);
	else
		stLog.DispLevel = DispLev;
	stLog.DispLevel = DispLev;
	stLog.FileLevel = FileLev;
}

void setSimLogFile ( char *cpPath, char *cpFilename)
{
	sprintf((char *)stLog.LogFile, "%s/%s", cpPath, cpFilename );
	log_write( DISP_LEV_SUCCESS | FILE_LEV_SUCCESS, "Start log [%s]", stLog.LogFile);
}

void setSimColor(bool ColorFlag)
{
#if 0
	if ( ColorFlag ) {
		strcpy(SETCOLOR_SUCCESS, DEF_SETCOLOR_SUCCESS);
		strcpy(SETCOLOR_FAILURE, DEF_SETCOLOR_FAILURE);
		strcpy(SETCOLOR_WARNING, DEF_SETCOLOR_WARNING);
		strcpy(SETCOLOR_NORMAL, DEF_SETCOLOR_NORMAL);
		printf("%s%s%s\n",SETCOLOR_SUCCESS, "SET COLOR", SETCOLOR_NORMAL);
	}
	else {
		strcpy(SETCOLOR_SUCCESS, DEF_SETCOLOR_NORMAL);
		strcpy(SETCOLOR_FAILURE, DEF_SETCOLOR_NORMAL);
		strcpy(SETCOLOR_WARNING, DEF_SETCOLOR_NORMAL);
		strcpy(SETCOLOR_NORMAL, DEF_SETCOLOR_NORMAL);
		printf("%s%s%s\n",SETCOLOR_SUCCESS, "DEFAULT COLOR", SETCOLOR_NORMAL);
	}
#endif
	return;
}

FLK_RETURN SimLogStart( bool ColorFlag, u_char DispLev, u_char FileLev, char *cpPath, char *cpFilename)
{
	bzero(&stLog, sizeof(log_st));
	setSimColor(ColorFlag);
	setSimLogLevelReset ( DispLev, FileLev);
	if ( cpPath != NULL && cpFilename != NULL)
		setSimLogFile(cpPath, cpFilename);
	return FLK_SUCCESS;
}
