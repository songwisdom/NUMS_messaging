/***********************************************************
*
* $Header: /home/cvs/smsc/skt-smsc/include/ksig7/IdLib.h,v 1.1 2008/07/21 05:12:49 prowoni Exp $
*
* $Source: /home/cvs/smsc/skt-smsc/include/ksig7/IdLib.h,v $
* $Revision: 1.1 $
* $Locker:  $
* 
***********************************************************/
/*
// Filename: IdLib.h 
// Creation date: 22/2/94
// Creator: BW
// Purpose:	Generate unique integers
*/

#ifndef IDLIB_H
#define IDLIB_H

/* Required files */
#include "ainCommon.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Defines */

#define ID_INVALID_ID	-1
#define ID_MIN_ID		0
#define ID_MAX_ID		0x7ffffffe

/* Types and structures */

typedef int ID_UniqueId;

/* API functions */

extern ID_UniqueId ID_get_id();
extern char *ID_id_to_str(ID_UniqueId id);
extern ID_UniqueId ID_str_to_id(char *str);

/* Error returns */
extern int id_errno;

#define ID_ERR_BAD_STR	1
#define ID_ERR_BAD_ID	2

#ifdef __cplusplus
}	/* extern "C" */
#endif

#endif  /* IDLIB_H */
