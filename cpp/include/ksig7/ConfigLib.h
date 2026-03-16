/***********************************************************
*
* $Header: /home/cvs/smsc/skt-smsc/include/ksig7/ConfigLib.h,v 1.1 2008/07/21 05:12:47 prowoni Exp $
*
* $Source: /home/cvs/smsc/skt-smsc/include/ksig7/ConfigLib.h,v $
* $Revision: 1.1 $
* $Locker:  $
* 
***********************************************************/
/*
// Filename: configlib.h
// Creation date: 7/1/93
// Creator: BW
// Purpose:	Exported header file for configuration library API
*/

#ifndef CONFIGLIB_H
#define CONFIGLIB_H

/* Required files */
#ifdef COCA_WIN32
#include <windows.h>
#endif /* COCA_WIN32 */
#include "ainCommon.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Defines */
#define ENVIRON_CFG_FILE "HP_AIN_CONFIG_FILE"
#define ENVIRON_HP_AIN_ROOT "HP_AIN_ROOT"
#ifdef COCA_WIN32
#define DEFAULT_GLOBAL_FILENAME "config\\global.conf"		/* Note that $HP_AIN_ROOT is added to start of this */
#define DEFAULT_LOCAL_FILENAME ".\\global.conf"
/* 
   Warning : DEFAULT_HP_AIN_ROOT is made of SystemDrive environment variable.
              To get the true path, you must transform DEFAULT_HP_AIN_ROOT with 
			  ExpandEnvironmentStrings first.
*/
#define DEFAULT_HP_AIN_ROOT "%SystemDrive%\\Program Files\\Hewlett-Packard\\HP-AIN\\"
#else
#define DEFAULT_GLOBAL_FILENAME "config/global.conf"		/* Note that $HP_AIN_ROOT is added to start of this */
#define DEFAULT_LOCAL_FILENAME "./global.conf"
#define DEFAULT_HP_AIN_ROOT	"/opt/HP-AIN/"
#endif

#define MAXLEN_CLASSNAME	80
#define MAXLEN_KEYNAME		80
#define MAXLEN_KEY			255

/* Types and structures */

/* API functions */

extern char *CFG_get_hpain_root();
extern int CFG_init(char *);
extern int CFG_close(int);
extern int CFG_get_classes(int, char *, int, char **);
extern int CFG_get_key_value(int, char *, char *, int, char **, char **);
extern int CFG_set_key_value(int, char *, int, char **, char **);

/* Error returns */

extern int cfg_errno;


#define CFG_NOFILE			1	/* Could not find, or fstat file */
#define CFG_PERM			2	/* Found file, but could not open for reading */
#define CFG_NOSUCHNAME		3	/* Could not find class or key value in file */
#define CFG_NOTINITIALISED	4	/* CFG_init not done for this id */
#define CFG_BADFILE			5	/* General error in parsing a line in the file (only if no specific error) */
#define CFG_NOCLASSES		6	/* No class sections found in file */
#define CFG_DUPCLASS		7	/* Duplicate class name found in file */
#define CFG_EMPTYFILE		8	/* File had zero length */
#define CFG_NOMEM			9	/* Malloc failed */
#define CFG_READERROR		10	/* file system error loading file */
#define CFG_KEYWITHNOCLASS	11	/* A valid key line was found outwith a class section */

#ifdef COCA_WIN32
/* Library entry point */
extern bool ConfigLib_DllMain (HANDLE hInst, 
                            ULONG ul_reason_for_call,
                            LPVOID lpReserved);
#endif 

#ifdef __cplusplus
}	/* extern "C" */
#endif


#endif  /* CONFIGLIB_H */
















