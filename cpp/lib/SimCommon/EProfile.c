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
*       MODULE NAME          : EProfile.c
*       PURPOSE              : Expand Profile.
*       INPUT ARGUMENT       : NONE
*       OUTPUT ARGUMENT      :
*       RETURN VALUE         : NONE
*       REVISION             :     BY             WHEN            REASON
*                             -------------  --------------  -------------
*                              KANG HYUN KWAN  1997.08.25.     first coding
==============================================================================*/
#include "FLKSimBaseDef.h"
#include "EProfile.h"

char cpINIFileName[SIM_PATH_MAX];
#define COMMENT_CHAR '#'
#define SPECIAL_CHAR '\\'
char *ECvtEnv2NormalStr(char *cpSour,char *cpTarget)
{
    char cpBuf[3][BUF_SSIZE];
    if(strstr(cpSour,"$(")==NULL)
    {
        strcpy(cpTarget,cpSour);
    }
    else
    if(sscanf(cpSour,"(%s)%s",cpBuf[0],cpBuf[1])==FLK_FAIL)
    {
        strcpy(cpTarget,cpSour);
    }
    else
        sprintf(cpTarget,"%s%s",getenv(cpBuf[0]),cpBuf[1]);
    return cpTarget;
}
char *StrUpr(char *cpStr)
{
	int nLoop;

	for(nLoop=0;cpStr[nLoop]!='\0';nLoop++)
		if(cpStr[nLoop]>='a'&&cpStr[nLoop]<='z')
			cpStr[nLoop]-=0x20;

	return cpStr;
}

int HexStr2Dec(char *cpHex)
{
	char cpBuf[BUF_SSIZE];
	int nLen,nLoop;
	int nResult=0,nMultiple=1;
	strcpy(cpBuf,cpHex);
	StrUpr(cpBuf);
	if(strncmp(cpBuf,"0X",2)==0)
		strcpy(cpBuf,cpBuf+2);
	nLen = (int)strlen(cpBuf);
	for(nLoop=nLen-1;nLoop>=0;nLoop--)
	{
		if(cpBuf[nLoop]>='0'&&cpBuf[nLoop]<='9')
			nResult += ((cpBuf[nLoop]-'0')*nMultiple);
		else
		if(cpBuf[nLoop]>='A'&&cpBuf[nLoop]<='F')
			nResult += ((cpBuf[nLoop]-'A'+10)*nMultiple);
		nMultiple *=16;
	}
	return nResult;
}
int EEraseComment(char *cpStr)
{
    int nLoop;
    int bSw=false;
    for(nLoop=0;cpStr[nLoop]!='\0';nLoop++) {
        if(!bSw) {
            if(cpStr[nLoop]=='\\') {
                bSw=true;
                continue;
            }
        }
        if(!bSw) {
            if(cpStr[nLoop]==COMMENT_CHAR)
                break;
        }
        bSw=false;
    }
    cpStr[nLoop]='\0';
    return nLoop;
}


int EEraseSpecialChar(char *cpStr)
{
    int nLoop,nLoop2;
    int bSw=false;
    char cpBuf[BUF_SSIZE];

    for(nLoop=0,nLoop2=0;cpStr[nLoop]!='\0';nLoop++) {
        if(!bSw) {
            if(cpStr[nLoop]=='\\') {
                bSw=true;
                continue;
            }
        }
        if(!bSw) cpBuf[nLoop2++] = cpStr[nLoop];
        else bSw=false;
    }
    cpBuf[nLoop2] = '\0';
    strcpy(cpStr,cpBuf);
    return nLoop2;
}

int EEraseCommentSpecialChar(char *cpStr)
{
    int re;
    EEraseComment(cpStr);
    re =  EEraseSpecialChar(cpStr);
    return re;
}
char *TrimRight(char *cpStr)
{
    int nLen,nI;
    nLen = (int)strlen(cpStr);
    for(nI=nLen-1;(cpStr[nI]==' '||cpStr[nI]=='\t'||cpStr[nI]=='\n')&&nI>=0;nI--);
    cpStr[nI+1] = '\0';
    return cpStr;
}
char *TrimLeft(char *cpStr)
{
    int nI,nJ;
    for(nI=0;cpStr[nI]==' '||cpStr[nI]=='\t';nI++);
    for(nJ=0;cpStr[nI]!='\0';nI++,nJ++)
        cpStr[nJ] = cpStr[nI];
    cpStr[nJ] = '\0';
    return cpStr;
}
char *Trim(char *cpStr)
{
    TrimLeft(cpStr);
    TrimRight(cpStr);
    return cpStr;
}


void TrimCh(char *cpStr,char cCh)
{
	int nLoop;
	for(nLoop=0;cpStr[nLoop]!=cCh&&cpStr[nLoop]!='\0';nLoop++);
		cpStr[nLoop]='\0';
	Trim(cpStr);
	return;
}
FILE *Fopen(char *cpFileName,char *mode)
{
    FILE *spFp;

    spFp = fopen(cpFileName,mode);
    return spFp;
}
FILE *LockFopen(char *cpFileName,char *cpMode)
{
    FILE *fp;
    if(cpMode[0]=='r')
        if(access(cpFileName,0)!=0)
		{
            return NULL;
		}
    fp = Fopen(cpFileName,cpMode);
    if(fp==NULL)
        return NULL;
    lockf(fileno(fp),F_LOCK,1l);
    return fp;
}
void LockFclose(FILE *fp)
{
    fseek(fp,SEEK_SET,0l);
    lockf(fileno(fp),F_ULOCK,1l);
    fclose(fp);
}
/*-------------------------------------------------------------------------
*       FUNCTION NAME        : ReadProfile()
*       PURPOSE              : read profile
*       INPUT ARGUMENT       :
*               1) cpAppName        : Application name
*               2) cpKeyName        : Key name 
*               3) cpReturnedString : returned string
*
*       RETURN VALUE         : int ; FLK_SUCCESS or FLK_FAIL
*       REVISION             :     BY             WHEN            REASON
*                              -------------  --------------  -------------
*                              KANG HYUN KWAN  1997.08.24.     first coding
----------------------------------------------------------------------------*/
int ReadProfile(char *cpAppName, char *cpKeyName, char *cpReturnedString)
{
    FILE *spFp;
    int bFindAppname = false;
    char cpBuf[BUF_SSIZE],cpBuf2[BUF_SSIZE];
    char _cpAppName[BUF_SSIZE],_cpKeyName[BUF_SSIZE];

    sprintf(_cpAppName,"[%s]",cpAppName);
    sprintf(_cpKeyName,"%s",cpKeyName);

    spFp = LockFopen(cpINIFileName,"r+");

    if(spFp==NULL)
	{
        return FLK_FAIL;
	}

    for(;;)
    {
		//iLineCommants = false;
		bzero(cpBuf, sizeof(cpBuf));
        fgets(cpBuf, BUF_SSIZE, spFp);
        if(feof(spFp))
            break;

		if (EEraseCommentSpecialChar(cpBuf) == FLK_SUCCESS)
			continue;
		
        if((strchr(cpBuf,'[')!=NULL)&&((strchr(cpBuf,']')!=NULL)))
            bFindAppname = false;

        if(strstr(cpBuf,_cpAppName)!=NULL)
        {
            bFindAppname = true;
            continue;
        }
		strcpy(cpBuf2,cpBuf);
		TrimCh(cpBuf2,'=');
        if(bFindAppname && strstr(cpBuf2,_cpKeyName)!=NULL)
        {
            strcpy(cpReturnedString,strstr(cpBuf,"=")+1);
            Trim(cpReturnedString);
            LockFclose(spFp);
            return FLK_SUCCESS;
        }
    }
    LockFclose(spFp);
    return FLK_FAIL;
}

/*-------------------------------------------------------------------------
*       FUNCTION NAME        : WriteProfile()
*       PURPOSE              : read profile
*       INPUT ARGUMENT       :
*               1) cpAppName  : Application name
*               2) cpKeyName  : Key name 
*               3) cpString   : string
*
*       RETURN VALUE         : int ; FLK_SUCCESS or FLK_FAIL
*       REVISION             :     BY             WHEN            REASON
*                              -------------  --------------  -------------
*                              KANG HYUN KWAN  1997.08.24.     first coding
----------------------------------------------------------------------------*/
void WriteProfile(char *cpAppName,char *cpKeyName,char *cpString)
{
	return ;
	/*
    FILE *spFp;
    int bFindAppname = false,bWrote = false,bSave = false;
    char cpBuf[BUF_SIZE],cpBuf2[BUF_SSIZE];
    char _cpAppName[BUF_SSIZE];
	char _cpKeyName[BUF_SSIZE],_cpKeyName2[BUF_SSIZE], _cpKeyName3[BUF_SSIZE];
	LONG lnPos,lnPosSave;
	int nLen,nLenSave,nLineCnt=0,nOldCnt=0;

	return;

#ifndef _HP_64BIT_SYS_TYPE_

    sprintf(_cpAppName,"[%s]",cpAppName);
    sprintf(_cpKeyName,"%s=",cpKeyName);
    sprintf(_cpKeyName2,"%s ",cpKeyName);
    sprintf(_cpKeyName3,"%s\t",cpKeyName);

    spFp = LockFopen(cpINIFileName,"r+");

    if(spFp==NULL)
    {
        spFp = LockFopen(cpINIFileName,"w");
		if(spFp==NULL)
		{
			OutError("Cannot write file (%s)\n",cpINIFileName);
			return;
		}
        fprintf(spFp,"%s\n",_cpAppName);
        fprintf(spFp,"%s=%s\n",cpKeyName,cpString);
        LockFclose(spFp);
        return;
    }
    for(;;)
    {
		lnPos = ftell(spFp);
		Fgets(cpBuf,BUF_SIZE,spFp);
		nLen = GetFgetsReadLen();

        if(feof(spFp))
            break;
		if(cpBuf[0]=='\n')
			nLineCnt++;
		else
		{
			nOldCnt=nLineCnt;
			nLineCnt=0;
		}
        if(cpBuf[0]=='#')
            continue;

        if(bFindAppname)
        {
			if((cpBuf[0]=='\n')&&(!bSave))
			{
				lnPosSave = lnPos;
				nLenSave = nLen;
				bSave = true;
				nLineCnt =0;
			}
            if((strchr(cpBuf,'[')!=NULL)&&(strchr(cpBuf,']')!=NULL))
            {

                sprintf(cpBuf2,"%s=%s\n",cpKeyName,cpString);
				if(bSave)
				{
					FInsert(spFp,lnPosSave,cpBuf2,nLenSave,strlen(cpBuf2));
					if(nOldCnt==0)
					{
						FInsert(spFp,lnPos+strlen(cpBuf2) - nLenSave,"\n",0,1);
					}
				}
				else
				{
					strcat(cpBuf2,"\n");
					strcat(cpBuf2,cpBuf);
					FInsert(spFp,lnPos,cpBuf2,nLen,strlen(cpBuf2));
				}
                bFindAppname = false;
                bWrote = true;
            } 
            else if( (strstr(cpBuf,_cpKeyName)!=NULL) || 
				(strstr(cpBuf,_cpKeyName2)!=NULL) ||
				(strstr(cpBuf,_cpKeyName3)!=NULL))
            {
                sprintf(cpBuf2,"\t%s = %s\n",cpKeyName,cpString);
				FInsert(spFp,lnPos,cpBuf2,nLen,strlen(cpBuf2));
                bFindAppname = false;
                bWrote = true;
            } 
        } 
		else
            if((strstr(cpBuf,_cpAppName)!=NULL)&&(!bWrote))
                bFindAppname = true;
    } //  end of for 

    if(!bWrote)
    {
		strcpy(cpBuf2,"");
        if(!bFindAppname)
        {
            if(nLineCnt==0)
			{
                strcpy(cpBuf2,"\n");
			}
            strcat(cpBuf2,_cpAppName);
			strcat(cpBuf2,"\n");
        }
        sprintf(cpBuf,"\t%s = %s\n",cpKeyName,cpString);
		strcat(cpBuf2,cpBuf);
		if(bSave)
			FInsert(spFp,lnPosSave,cpBuf2,nLenSave,strlen(cpBuf2));
		else
			FInsert(spFp,lnPos,cpBuf2,nLen,strlen(cpBuf2));
    }
    LockFclose(spFp);
#endif
	*/
}


/*-------------------------------------------------------------------------
*       FUNCTION NAME        : GetProfile()
*       PURPOSE              : read profile if not exsit then write profile
*       INPUT ARGUMENT       :
*               1) cpAppName        : Application name
*               2) cpKeyName        : Key name 
*               3) cpReturnedString : returned string
*               4) cpDefault        : default string
*
*       RETURN VALUE         : char  * ; string of result
*       REVISION             :     BY             WHEN            REASON
*                              -------------  --------------  -------------
*                              KANG HYUN KWAN  1997.08.24.     first coding
----------------------------------------------------------------------------*/
char *
GetProfile(char *cpAppName, char *cpKeyName,
				 char *cpReturnedString, char *cpDefault)
{
    if (ReadProfile(cpAppName, cpKeyName, cpReturnedString) == FLK_FAIL)
    {
        if(NULL == cpDefault) return NULL;
        strcpy(cpReturnedString, cpDefault);
         
    }
    return cpReturnedString;
}

int 
GetProfileInt(char *cpAppName,char *cpKeyName,int nDefault)
{
	char cpReturnedString[BUF_SSIZE];

    if (FLK_FAIL == ReadProfile(cpAppName,cpKeyName,cpReturnedString))
    {
        return nDefault;
    }

	if(strncmp(cpReturnedString,"0x",2)==0)
		return (int)HexStr2Dec(cpReturnedString);

	return atoi(cpReturnedString);
}

int GetProfile1(char *cpAppName,char *cpKeyName,
                 char *cpReturnedString,char *cpDefault)
{
    if (FLK_FAIL == ReadProfile(cpAppName,cpKeyName,cpReturnedString))
        return(-1);

    return(1);
}

int 
GetProfileInt1(char *cpAppName,char *cpKeyName, int *npReturnInt, int nDefault)
{
    char cpReturnedString[BUF_SSIZE];

    if (FLK_FAIL == ReadProfile(cpAppName,cpKeyName,cpReturnedString))
        return(-1);

    if(strncmp(cpReturnedString,"0x",2)==0)
        *npReturnInt = (int)HexStr2Dec(cpReturnedString);
    else *npReturnInt = atoi(cpReturnedString);
    return(1);
}

int 
GetProfileLong(char *cpAppName,char *cpKeyName,int lnDefault)
{
	char cpReturnedString[BUF_SSIZE];

    if (FLK_FAIL == ReadProfile(cpAppName,cpKeyName,cpReturnedString))
        if (lnDefault) sprintf(cpReturnedString,"%d",lnDefault);

	if (!strncmp(cpReturnedString,"0x",2) || !strncmp(cpReturnedString,"0X",2))
		return (int)HexStr2Dec(cpReturnedString);
	else
		return atoi(cpReturnedString);
}

int 
GetProfileHex(char *cpAppName,char *cpKeyName,char *cpHexDefault)
{
	char cpReturnedString[BUF_SSIZE];
     
    if (FLK_FAIL == ReadProfile(cpAppName,cpKeyName,cpReturnedString))
    {
        if (NULL != cpHexDefault)
            strcpy(cpReturnedString, cpHexDefault);
    }

	if (!strncmp(cpReturnedString,"0x",2) || !strncmp(cpReturnedString,"0X",2))
		return (int)HexStr2Dec(cpReturnedString);

	return atoi(cpReturnedString);
}

void 
GetProfileFileName(char *cpAppName,char *cpKeyName,
				 char *cpReturnedString,char *cpDefault)
{
    char cpBuf[BUF_SSIZE];
    GetProfile(cpAppName,cpKeyName,cpBuf,cpDefault);
    ECvtEnv2NormalStr(cpBuf,cpReturnedString);
}

/*-------------------------------------------------------------------------
*       FUNCTION NAME        : SetInitName()
*       PURPOSE              : set ini file name
*       INPUT ARGUMENT       :
*               1) cpFileName  : ini file name
*       REVISION             :     BY             WHEN            REASON
*                              -------------  --------------  -------------
*                              KANG HYUN KWAN  1997.08.24.     first coding
----------------------------------------------------------------------------*/
void SetIniName(char *cpFileName)
{
	bzero(cpINIFileName, sizeof(cpINIFileName));
    strcpy(cpINIFileName,cpFileName);
}

char *GetIniName(void)
{
    return cpINIFileName;
}

void 
WriteProfileInt(char *cpAppName,char *cpKeyName,int nValue)
{
	char cpBuf[BUF_SSIZE];
	sprintf(cpBuf,"%d",nValue);
	WriteProfile(cpAppName,cpKeyName,cpBuf);
}

void 
WriteProfileLong(char *cpAppName,char *cpKeyName,int lnValue)
{
	char cpBuf[BUF_SSIZE];
	sprintf(cpBuf,"%d",lnValue);
	WriteProfile(cpAppName,cpKeyName,cpBuf);
}

void 
WriteProfileHex(char *cpAppName,char *cpKeyName,char *cpHexValue)
{
	WriteProfile(cpAppName,cpKeyName,cpHexValue);
}
/** @} */
