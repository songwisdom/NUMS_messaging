/*===============================================================================
*       MODULE NAME          : EProfile.h
*       PURPOSE              : Expand Profile.
*       INPUT ARGUMENT       : NONE
*       OUTPUT ARGUMENT      : NONE
*       RETURN VALUE         : NONE
*       REVISION             :     BY             WHEN            REASON
*                             -------------  --------------  -------------
*                                seoki        2001.11.24      copy cat 
==============================================================================*/

#ifndef __EPROFILE_H__
#define __EPROFILE_H__

#include "FLKSimBaseDef.h"
#include "flk_type.h"
#define SIM_PATH_MAX 1024
extern char cpINIFileName[SIM_PATH_MAX];

#ifdef __cplusplus
	extern "C" {
#endif

int ReadProfile(char *cpAppName, char *cpKeyName, char *cpReturnedString);
void WriteProfile(char *cpAppName,char *cpKeyName,char *cpString);
char * GetProfile(char *cpAppName, char *cpKeyName, char *cpReturnedString, char *cpDefault);
int GetProfileInt(char *cpAppName,char *cpKeyName,int nDefault);
int GetProfile1(char *cpAppName,char *cpKeyName, char *cpReturnedString,char *cpDefault);
int GetProfileInt1(char *cpAppName,char *cpKeyName, int *npReturnInt, int nDefault);
int GetProfileLong(char *cpAppName,char *cpKeyName,int lnDefault);
int GetProfileHex(char *cpAppName,char *cpKeyName,char *cpHexDefault);
void GetProfileFileName(char *cpAppName,char *cpKeyName, char *cpReturnedString,char *cpDefault);
void SetIniName(char *cpFileName);
char *GetIniName(void);
void WriteProfileInt(char *cpAppName,char *cpKeyName,int nValue);
void WriteProfileLong(char *cpAppName,char *cpKeyName,int lnValue);
void WriteProfileHex(char *cpAppName,char *cpKeyName,char *cpHexValue);

#ifdef __cplusplus
}
#endif

#endif /* __EPROFILE_H__  */

