/***********************************************************************
*                                                                      *
*   Copyright (c) 1997 LG Information & Communications                 *
*   All rights reserved.                                               *
*                                                                      *
*   This program contains confidential and proprietary information of  *
*   LGIC, and any reproduction, disclosure, or use in whole or in part *
*   is expressly  prohibited, except as may be specifically authorized *
*   by prior written agreement or permission of LGIC.                  *
*                                                                      *
*               LG Information & Communications, Ltd.                  *
*        395-65 , Shindaebang-dong, Dongjak-gu, Seoul, 150-010 Korea   *
*                                                                      *
***********************************************************************/
/** 
 @ingroup Expand 
 @{
*/
/*===============================================================================
*       MODULE NAME          : EIo.c
*       PURPOSE              : Expand I/O.
*       INPUT ARGUMENT       : NONE
*       OUTPUT ARGUMENT      :
*       RETURN VALUE         : NONE
*       REVISION             :     BY             WHEN            REASON
*                             -------------  --------------  -------------
*                              KANG HYUN KWAN  1997.08.25.     first coding
==============================================================================*/
#include "flk_type.h"
#include "EIo.h"
#include "unistd.h"

/*
#include <curses.h>
*/



/*-------------------------------------------------------------------------
*       FUNCTION NAME        : LockFopen()
*       PURPOSE              : open file and lock
*       INPUT ARGUMENT       :
*               1) cpFileName  : open file name
*               2) cpMode      : open mode ( cannot use "r" )
*
*       RETURN VALUE         : FILE * ; file pointer
*       REVISION             :     BY             WHEN            REASON
*                              -------------  --------------  -------------
*                              KANG HYUN KWAN  1997.08.24.     first coding
----------------------------------------------------------------------------*/
FILE *LockFopen(char *cpFileName,char *cpMode)
{
    FILE *fp;
    if(cpMode[0]=='r')
        if(access(cpFileName,0)!=0)
		{
			SetErrorNo(EEFILENOTFOUND,"LockFopen()");
            return NULL;
		}
    fp = Fopen(cpFileName,cpMode);
    if(fp==NULL)
        return NULL;
    lockf(fileno(fp),F_LOCK,1l);
    return fp;
}

/*-------------------------------------------------------------------------
*       FUNCTION NAME        : LockFclose()
*       PURPOSE              : unlock and close file
*       INPUT ARGUMENT       :
*               1) fp : file pointer
*       REVISION             :     BY             WHEN            REASON
*                              -------------  --------------  -------------
*                              KANG HYUN KWAN  1997.08.24.     first coding
----------------------------------------------------------------------------*/
void LockFclose(FILE *fp)
{
    fseek(fp,SEEK_SET,0l);
    lockf(fileno(fp),F_ULOCK,1l);
    fclose(fp);
}






















//////////////////////////////////////////////////////////////////////////
#define DEFAULT_ROWS 24
#define GET_CHAR_RAW_MAXBUF 1000
#define STR_RAW_PROMPT_SIZE 100
#define KEY_HISTORY_BUF_SIZE 200
/****************************************************************************
    TTY function
****************************************************************************/
static struct termios save_termios;
static int ttysavefd = -1;
static enum {RESET,RAW,CBREAK } ttystate = RESET,oldttystate;
/******************** End of TTY *******************************************/

static char StrRawPrompt[STR_RAW_PROMPT_SIZE] ="";
static char cpKeyHistoryBuf[KEY_HISTORY_BUF_SIZE][STR_RAW_PROMPT_SIZE];
static int nKeyHistoryCnt=0;

int _GetCharRawBuf[GET_CHAR_RAW_MAXBUF];
int _GetCharRawHead = 0;
int _GetCharRawTail = 0;

static int InsertMode = false;

void SaveKeyHistoryBuf(char *cpAppName)
{
	int nLoop;
	char cpBuf[BUF_SSIZE];
	WriteProfileInt(cpAppName,"BufCnt",nKeyHistoryCnt);
	for(nLoop=0;nLoop<nKeyHistoryCnt;nLoop++)
	{
		sprintf(cpBuf,"Str%03d",nLoop);
		WriteProfile(cpAppName,cpBuf,cpKeyHistoryBuf[nLoop]);
	}
}

void LoadKeyHistoryBuf(char *cpAppName)
{
	int nLoop;
	char cpBuf[BUF_SSIZE],cpBuf2[BUF_SSIZE];
	nKeyHistoryCnt = GetProfileInt(cpAppName,"BufCnt",0);
	for(nLoop=0;nLoop<nKeyHistoryCnt;nLoop++)
	{
		sprintf(cpBuf,"Str%03d",nLoop);
		GetProfile(cpAppName,cpBuf,cpBuf2,"");
		strcpy(cpKeyHistoryBuf[nLoop],cpBuf2);
	}

}

FILE *Fopen(char *cpFileName,char *mode)
{
    FILE *spFp;
	DPRT(("file name[%s] mode[%s]\n", cpFileName, mode));
    spFp = fopen(cpFileName,mode);
    if(spFp==NULL)
    {
		SetErrorNo(EEFILEOPEN,"Fopen()");
    }
    return spFp;
}

FILE *Fdopen(int nfd,char *mode)
{
    FILE *spFp;
    spFp = fdopen(nfd,mode);
    if(spFp==NULL)
    {
		SetErrorNo(EEFILEOPEN,"Fdopen()");
    }
    return spFp;
}

void Fclose(FILE *fp)
{
    fclose(fp);
}



/*-------------------------------------------------------------------------
*       FUNCTION NAME        : SetTtyCbreak()
*       PURPOSE              : Set TTY cbreak
*       INPUT ARGUMENT       :
*               1) fd  : file description
*
*       RETURN VALUE         : int ; 
*       REVISION             :     BY             WHEN            REASON
*                              -------------  --------------  -------------
*                              KANG HYUN KWAN  1997.09.09.     first coding
----------------------------------------------------------------------------*/
int SetTtyCbreak(int nFd)
{
    struct termios buf;
    if(tcgetattr(nFd,&save_termios)<0)
	{
		SetErrorNo(EETCGETATTR,"SetTtyCbreak()");
        return FLK_FAIL;
	}
    buf = save_termios;
    buf.c_lflag &= ~(ECHO|ICANON); /* echo off, canonical mode off */
    buf.c_cc[VMIN] = 1; /* Case b: 1 byte at a time, no timer */
    buf.c_cc[VTIME] =0;
    if(tcsetattr(nFd,TCSAFLUSH,&buf)<0)
	{
    	SetErrorNo(EETCGETATTR,"SetTtyCbreak()");
        return FLK_FAIL;
	}
    oldttystate = ttystate;
    ttystate = CBREAK;
    ttysavefd = nFd;
    return FLK_SUCCESS;
}

/*-------------------------------------------------------------------------
*       FUNCTION NAME        : SetTtyRaw()
*       PURPOSE              : Set TTY Raw
*       INPUT ARGUMENT       :
*               1) fd  : file description
*
*       RETURN VALUE         : int ; 
*       REVISION             :     BY             WHEN            REASON
*                              -------------  --------------  -------------
*                              KANG HYUN KWAN  1997.09.09.     first coding
----------------------------------------------------------------------------*/
int SetTtyRaw(int nFd)
{
    struct termios buf;
    if(tcgetattr(nFd,&save_termios)<0)
	{
		SetErrorNo(EETCGETATTR,"SetTtyRaw()");
        return FLK_FAIL;
	}
    buf = save_termios;
    buf.c_lflag &= ~(ECHO|ICANON|IEXTEN|ISIG);
    buf.c_iflag &= ~(BRKINT|ICRNL|INPCK|ISTRIP|IXON);
    buf.c_cflag &= ~(CSIZE|PARENB);
    buf.c_cflag |= CS8;
    buf.c_oflag &= ~(OPOST);
    buf.c_cc[VMIN] = 1;
    buf.c_cc[VTIME] =0;
    if(tcsetattr(nFd,TCSAFLUSH,&buf)<0)
	{
		SetErrorNo(EETCGETATTR,"SetTtyRaw()");
        return FLK_FAIL;
	}
    oldttystate = ttystate;
    ttystate = RAW;
    ttysavefd = nFd;
    return FLK_SUCCESS;
}

/*-------------------------------------------------------------------------
*       FUNCTION NAME        : ResetTty()
*       PURPOSE              : Reset TTY
*       INPUT ARGUMENT       :
*               1) nFd  : file description
*
*       RETURN VALUE         : int ; 
*       REVISION             :     BY             WHEN            REASON
*                              -------------  --------------  -------------
*                              KANG HYUN KWAN  1997.09.09.     first coding
----------------------------------------------------------------------------*/
int ResetTty(int nFd)
{
    if(ttystate !=CBREAK && ttystate !=RAW)
        return FLK_SUCCESS;
    if(tcsetattr(nFd,TCSAFLUSH,&save_termios)<0)
	{
		SetErrorNo(EETCGETATTR,"ResetTty()");
        return FLK_FAIL;
	}
    oldttystate = ttystate;
    ttystate = RESET;
    return FLK_SUCCESS;
}

/*-------------------------------------------------------------------------
*       FUNCTION NAME        : SetOldTtyState()
*       PURPOSE              : Set old TTY state
*       INPUT ARGUMENT       :
*               1) nFd  : file description
*
*       RETURN VALUE         : int ; 
*       REVISION             :     BY             WHEN            REASON
*                              -------------  --------------  -------------
*                              KANG HYUN KWAN  1997.09.09.     first coding
----------------------------------------------------------------------------*/
int SetOldTtyState(int nFd)
{
    if(oldttystate == CBREAK)
        SetTtyCbreak(nFd);
    else
    if(oldttystate == RAW)
        SetTtyRaw(nFd);
    else
        ResetTty(nFd);
    return FLK_SUCCESS;
}

static void SigCatch(int nSigno)
{
    ResetTty(STDIN_FILENO);
    Exit(0);
}

int GetGetCharRawBufCnt(void)
{
    if(_GetCharRawTail < _GetCharRawHead)
        return _GetCharRawHead - _GetCharRawTail;
    else
    if(_GetCharRawTail == _GetCharRawHead)
        return 0;
    else
        return GET_CHAR_RAW_MAXBUF  - _GetCharRawTail + _GetCharRawHead;
}

void PushGetCharRawBuf(char *cpBuf,int nLen)
{
    int nI;
    for(nI=0;nI<nLen;nI++)
    {
        _GetCharRawBuf[_GetCharRawHead] = cpBuf[nI];
        _GetCharRawHead++;
        if(_GetCharRawHead >= GET_CHAR_RAW_MAXBUF)
            _GetCharRawHead = 0;
    }
}

int PopGetCharRawBuf(void)
{
    u_char cpCh[4];
    union 
    {
        char cpCh[4];
        int nI;
    } data;
    int nCnt,nCnt2=0;
    int nCode =0;
    int nI,nJ,nK;
    nCnt = GetGetCharRawBufCnt();
    for(nI=0;nI<nCnt;nI++)
    {
        cpCh[nI] = _GetCharRawBuf[_GetCharRawTail];
        if(nI==0)
        {
            if(_GetCharRawBuf[_GetCharRawTail]!=ESC)
            {
                nCode = _GetCharRawBuf[_GetCharRawTail];
                _GetCharRawTail++;
                if(_GetCharRawTail>=GET_CHAR_RAW_MAXBUF)
                    _GetCharRawTail = 0;
                break;
            }
            else
            {
                cpCh[nCnt2] = _GetCharRawBuf[_GetCharRawTail];
                nCnt2++;
                _GetCharRawTail++;
                if(_GetCharRawTail>=GET_CHAR_RAW_MAXBUF)
                    _GetCharRawTail = 0;
                if(_GetCharRawHead==_GetCharRawTail)
                    break;
            }
        }
        else
        {
            if(_GetCharRawBuf[_GetCharRawTail]!=ESC)
            {
                cpCh[nCnt2] = _GetCharRawBuf[_GetCharRawTail];
                nCnt2++;
                _GetCharRawTail++;
                if(_GetCharRawTail>=GET_CHAR_RAW_MAXBUF)
                    _GetCharRawTail = 0;
                if(_GetCharRawHead==_GetCharRawTail)
                    break;
            }
            else
                break;
        }
    }
    if(nCode==0)
    {
        memset(data.cpCh,0,4);
        for(nJ = 4 - nCnt2,nK=0;nJ<4;nJ++,nK++)
            data.cpCh[nJ] = cpCh[nK];
        return data.nI;
    }
    return nCode;
}

/*-------------------------------------------------------------------------
*       FUNCTION NAME        : GetCharRaw()
*       PURPOSE              : Get Char raw
*       INPUT ARGUMENT       :
*
*       RETURN VALUE         :  
*       REVISION             :     BY             WHEN            REASON
*                              -------------  --------------  -------------
*                              KANG HYUN KWAN  1997.09.09.     first coding
----------------------------------------------------------------------------*/

void (*signal(int sig, void (*func)(int)))(int);
int GetCharRaw(void)
{
    char cpBuf[BUF_SSIZE],cpBuf2[BUF_SSIZE];
    int oldptr[5];
    int nLen,nLen2=0;
	struct pollfd stPfd[1];

    if(GetGetCharRawBufCnt())
        return PopGetCharRawBuf();
	/*
#ifdef _HP_64BIT_SYS_TYPE_
    oldptr[0] = (int)signal(SIGINT,SigCatch);
    oldptr[1] = (int)signal(SIGQUIT,SigCatch);
    oldptr[2] = (int)signal(SIGTERM,SigCatch);
#else 
	oldptr[0] = (int)signal(SIGINT,SigCatch);
    oldptr[1] = (int)signal(SIGQUIT,SigCatch);
    oldptr[2] = (int)signal(SIGTERM,SigCatch);
#endif
	*/
    if(ttystate==RESET)
        SetTtyRaw(STDIN_FILENO);

    if((nLen = (int)read(STDIN_FILENO,cpBuf,BUF_SSIZE))<0)
    {
        SetOldTtyState(STDIN_FILENO);
        signal(SIGINT, signal(SIGINT, SigCatch));
        signal(SIGQUIT, signal(SIGQUIT, SigCatch));
        signal(SIGTERM,signal(SIGTERM, SigCatch));
		SetErrorNo(EEREAD,"GetCharRaw()");
        return FLK_FAIL;
    }
	if(cpBuf[0]==ESC && ((nLen<3)||((nLen==3)&& (cpBuf[2]==0x32))))
	{
		stPfd[0].fd = STDIN_FILENO;
		stPfd[0].events = POLLIN;
		poll(stPfd,1,500);
		if(stPfd[0].revents&POLLIN)
		{
			nLen2 = (int)read(STDIN_FILENO,cpBuf2,BUF_SSIZE);
		}
	}
    SetOldTtyState(STDIN_FILENO);
    signal(SIGINT, signal(SIGINT, SigCatch));
    signal(SIGQUIT, signal(SIGQUIT, SigCatch));
    signal(SIGTERM, signal(SIGTERM, SigCatch));

    PushGetCharRawBuf(cpBuf,nLen);
	if(nLen2>0)
		PushGetCharRawBuf(cpBuf2,nLen2);
    return PopGetCharRawBuf();
}

/*-------------------------------------------------------------------------
*       FUNCTION NAME        : SetStrRawPrompt()
*       PURPOSE              : Set Str raw prompt
*       INPUT ARGUMENT       :
*
*       RETURN VALUE         : 
*       REVISION             :     BY             WHEN            REASON
*                              -------------  --------------  -------------
*                              KANG HYUN KWAN  1997.09.09.     first coding
----------------------------------------------------------------------------*/
void SetStrRawPrompt(char *cpStr)
{
    strcpy(StrRawPrompt,cpStr);
}

void DisplayGetStrData(char *cpStr,int nPos)
{
	static char cpBeforStr[BUF_SSIZE]="";
    int bSw = false;
    int nLen,nLen2;
    char cpBuf[BUF_SSIZE],cpBuf2[BUF_SSIZE],cpBuf3[BUF_SSIZE];

    strcpy(cpBuf,cpStr);
    strcpy(cpBuf2,cpStr);
    cpBuf2[nPos]=NULL;
    nLen = (int)strlen(cpBeforStr);
    nLen2 = (int)strlen(cpStr);

    if(nLen>nLen2)
    {
        memset(cpBuf3,' ',nLen-nLen2);
        cpBuf3[nLen-nLen2]=NULL;
        strcat(cpBuf,cpBuf3);
    }
    fprintf(stdout,"\r%s\r%s",cpBuf,cpBuf2);
    fflush(stdout);
    strcpy(cpBeforStr,cpStr);
}

/*-------------------------------------------------------------------------
*       FUNCTION NAME        : GetStrRaw()
*       PURPOSE              : Get string raw
*       INPUT ARGUMENT       :
*
*       RETURN VALUE         :
*       REVISION             :     BY             WHEN            REASON
*                              -------------  --------------  -------------
*                              KANG HYUN KWAN  1997.09.09.     first coding
----------------------------------------------------------------------------*/
int GetStrRaw(char *cpBuf)
{
    char cpBuf2[BUF_SSIZE];
    int nPos,nPos2,ch,nI,nJ;
    int nKeyHisPos =  nKeyHistoryCnt;

    memset(cpBuf2,NULL,BUF_SSIZE);
    strcpy(cpBuf2,StrRawPrompt);
    nPos2 = (int)strlen(cpBuf2);
    nPos = nPos2;
    for(;;)
    {
        DisplayGetStrData(cpBuf2,nPos);
        ch = GetCharRaw();
        if(ch==CTRL_M)
        {
            break;
        }
        if((ch==EKEY_INSERT_STRATUS)||(ch==EKEY_INSERT_HP))
        {
            InsertMode = (InsertMode)? false: true;
            continue;
        }
         
        if(ch==ESC)
        {
            strcpy(cpBuf2,StrRawPrompt);
            nPos = (int)strlen(cpBuf2);
        }
        else
        if(ch==EKEY_LEFT)
        {
            if(nPos>nPos2)
                nPos--;
        }
        else
        if(ch==EKEY_RIGHT)
        {
            if(nPos<strlen(cpBuf2))
                nPos++;
        }
        else
        if(ch==EKEY_UP)
        {
            if(nKeyHisPos>0)
            {
                sprintf(cpBuf2,"%s%s",StrRawPrompt,cpKeyHistoryBuf[--nKeyHisPos]);
                nPos = (int)strlen(cpBuf2);
            }
        }
        else
        if(ch==EKEY_DOWN)
        {
            if(nKeyHisPos<nKeyHistoryCnt)
            {
                nKeyHisPos++;
                sprintf(cpBuf2,"%s%s",StrRawPrompt,cpKeyHistoryBuf[nKeyHisPos]);
                nPos = (int)strlen(cpBuf2);
            }
        }
        else
        if(ch==EKEY_DELETE)
        {
            if(nPos<strlen(cpBuf2))
            {
                for(nI=nPos+1;(cpBuf2[nI]!=NULL) && (nI<=BUF_SSIZE);nI++)
                    cpBuf2[nI-1] = cpBuf2[nI];
                cpBuf2[nI-1]=NULL;
            }
        }
        else
        if(ch==CTRL_H)
        {
            if(nPos>nPos2)
            {
                for(nI=nPos;(cpBuf2[nI]!=NULL) && (nI<=BUF_SSIZE);nI++)
                    cpBuf2[nI-1] = cpBuf2[nI];
                cpBuf2[nI-1]=NULL;
                nPos--;
            }
        }
        else
        if(ch==EKEY_HOME)
        {
            nPos = nPos2;
        }
        else
        if(ch==EKEY_END)
        {
            nPos = (int)strlen(cpBuf2);
        }
        else
        if(ch == EKEY_EXPAND_ARROW)
        {
            ch = GetCharRaw();
            if(ch==EKEY_EXPAND_ARROW_LEFT)
            {
                if(nPos>nPos2)
                    nPos--;
            }   
            else
            if(ch==EKEY_EXPAND_ARROW_RIGHT)
            {
                if(nPos<strlen(cpBuf2))
                    nPos++;
            }
            else
            if(ch==EKEY_EXPAND_ARROW_UP)
            {
                if(nKeyHisPos>0)
                {
                    sprintf(cpBuf2,"%s%s",StrRawPrompt,cpKeyHistoryBuf[--nKeyHisPos]);
                    nPos = (int)strlen(cpBuf2);
                }
            }
            else
            if(ch==EKEY_EXPAND_ARROW_DOWN)
            {
                if(nKeyHisPos<nKeyHistoryCnt)
                {
                    nKeyHisPos++;
                    sprintf(cpBuf2,"%s%s",StrRawPrompt,cpKeyHistoryBuf[nKeyHisPos]);
                    nPos = (int)strlen(cpBuf2);
                }
            }
        }
        else
        {

            if(InsertMode)
            {
                nJ = (int)strlen(cpBuf2);
                for(nI=(int)strlen(cpBuf2)+1;(nI>=nPos)&&(nI<=BUF_SSIZE);nI--)
                    cpBuf2[nI+1] = cpBuf2[nI];
            }
            if(cpBuf2[nPos]==NULL)
                cpBuf2[nPos+1] = NULL;
            cpBuf2[nPos] = ch;
            nPos++;
        }
    }
    strcpy(cpBuf,cpBuf2+nPos2);
    if(strlen(cpBuf)==0)
        return ESC;
    if(nKeyHistoryCnt>KEY_HISTORY_BUF_SIZE)
    {
        for(nI=0;nI<STR_RAW_PROMPT_SIZE;nI++)
            strcpy(cpKeyHistoryBuf[nI],cpKeyHistoryBuf[nI+STR_RAW_PROMPT_SIZE]);
        nKeyHistoryCnt = STR_RAW_PROMPT_SIZE;
    }
    strcpy(cpKeyHistoryBuf[nKeyHistoryCnt],cpBuf);
    nKeyHistoryCnt++;
    fprintf(stdout,"\r\n");
	fflush(stdout);
    return FLK_SUCCESS;
}

/*-------------------------------------------------------------------------
*       FUNCTION NAME        : GetRows()
*       PURPOSE              : Get rows
*       INPUT ARGUMENT       :
*
*       RETURN VALUE         : int ; rows
*       REVISION             :     BY             WHEN            REASON
*                              -------------  --------------  -------------
*                              KANG HYUN KWAN  1997.09.09.     first coding
----------------------------------------------------------------------------*/
int GetRows(void)
{
    struct winsize size;
    ioctl(STDIN_FILENO,TIOCGWINSZ,(char*)&size);
    if(size.ws_row==0)
        return DEFAULT_ROWS;
    return size.ws_row;
}
/** @} */
