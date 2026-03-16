/*===============================================================================
*       MODULE NAME          : EProcess.h
*       PURPOSE              : Expand Process.
*       INPUT ARGUMENT       : NONE
*       OUTPUT ARGUMENT      : NONE
*       RETURN VALUE         : NONE
*       REVISION             :     BY             WHEN            REASON
*                             -------------  --------------  -------------
*                                seoki        2001.11.24      copy cat
==============================================================================*/

#ifndef __EPROCESS_H__
#define __EPROCESS_H__


#ifdef __cplusplus
//extern "C" {
#endif
int Fork(void);
void Kill(int pid, int msg);
void Exit(int code);

#ifdef __cplusplus
//}
#endif

#endif /* __PROCESS_H__  */

