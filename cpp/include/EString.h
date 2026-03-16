/*===============================================================================
*       MODULE NAME          : EString.h
*       PURPOSE              : Expand String.
*       INPUT ARGUMENT       : NONE
*       OUTPUT ARGUMENT      : NONE
*       RETURN VALUE         : NONE
*       REVISION             :     BY             WHEN            REASON
*                             -------------  --------------  -------------
*                                seoki        2001.11.24      copy cat
==============================================================================*/

#ifndef __ESTRING_H__
#define __ESTRING_H__


//#ifdef __cplusplus
//extern "C" {
//#endif
char *TrimLeft(char *cpStr);
char *TrimRight(char *cpStr);
char *Trim(char *cpStr);
int GetArgvCount(char **cpArgv);
char **PutArgv(char *cpStr, int *nArgc);
void PutArgvFree(int nArgc, char **cpArgv);
void _PutArgvFree(int nArgc, char **cpArgv);
int CountChar(const char *cpStr, char ch);
char *ReplaceStr(const char *cpStr, char *cpBuf, const char *sour, const char *tar);
char *ReplaceStrs(const char *cpStr, char *cpBuf, const char *cpSour, const char *cpTar);
char *GetMidStr(const char *cpStr, char *cpBuf, int nStartPos, int nEndPos);
char *StrUpr(char *cpStr);
void StrCatCh(char *cpStr, char cCh);
int StrNCnt(const char *cpStr, char cCh);
int IsString(const char *cpStr, int nMaxLen);
int GetInt(const char *cpStr, int *nLen);
long GetLong(char *cpStr, int *nLen);
long GetHex(char *cpStr, int *nLen);
int CheckCharacters(const char *cpStr, int nCnt, char *cpChars, int *nPos);
void EFreeStr(char **cpStr, int nCnt);
int EPutBitByte(char *cpSourStr, int nStartPos, int nLen, int nData);
char GetEndChar(char *cpStr);

//#ifdef __cplusplus
//}
//#endif

#endif /* __ESTRING_H__  */

