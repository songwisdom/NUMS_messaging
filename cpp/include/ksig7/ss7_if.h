/*=====================================================================
|                       
|                               ss7_if.h
|                              ----------
|
| SS7 Application Programmatic Interface  definitions.
|
| $Source: /home/cvs/smsc/skt-smsc/include/ksig7/ss7_if.h,v $
| $Revision: 1.1 $
| $Date: 2008/07/21 05:12:48 $
| HEWLETT PACKARD SS7
| (c) Copyright 1991, Hewlett-Packard Company, all rights reserved
|
======================================================================*/

/*=====================================================================
|  General purpose defines for SS7 libraries
======================================================================*/

#ifndef SS7_API_H 
#define SS7_API_H

#ifdef _WIN32
#ifndef HP_DLL_IF
#define HP_DLL_IF __declspec(dllimport)
#endif /* HP_DLL_IF */ 
#else
#define HP_DLL_IF
#endif /* _WIN32 */

#include <sys/types.h>
#include "ainCommon.h"

#define WB_MAX_USERDATA      2048
#define REGULAR_MAX_USERDATA 255

#ifdef ANSI88
#define MAX_MTP_MSU_LEN 265
#define MAX_USERDATA    WB_MAX_USERDATA
#else
#define MAX_MTP_MSU_LEN 268
#define MAX_USERDATA    WB_MAX_USERDATA
#endif

#define MAX_GEN_MSU_LEN 700

#ifndef Ubool_DEFINED
#define Ubool_DEFINED
typedef enum { no, yes } unsignedbool;
#endif

/*for 64bit*/
//#if __STDC_VERSION__ >= 199901L
#if (!defined(__STDC_VERSION__)) || (__STDC_VERSION__ >= 199901L)
  /* "inline" is a keyword */
#else
# define inline /* nothing */
#endif // __STDC_VERSION__ end

#include <stdbool.h>
#include <inttypes.h>

typedef enum { active, standby } ss7_process;

/*=====================================================================
|  SS7 services types
======================================================================*/

typedef enum {
      ss7_oam_l2, 
      ss7_mtp,
      ss7_oam,
      ss7_oam_mtp, 
      ss7_proxy, 
      ss7_vod,
      ss7_sccp, 
      ss7_oam_sccp,
      ss7_tcap,
      /* NL: SIU_Mgt add new SIU stuff */
      ss7_oam_mtp2x,
      ss7_oam_reserved,
      ss7_tdx,
      ss7_intern /* it is for parallel isup use only */
} ss7_sce_type;

/* defines the maximum number of services */
/* MV - incremented from 12 to 13, because added a new sce: ss7_mtp_for_isup inss7_if_intern.h */
#define MAX_SCE_ID 13

#ifdef _WIN32
/*=====================================================================
| parameters definitions for ss7_startup call
======================================================================*/
typedef int ss7_startup_parameter;
#endif /* _WIN32 */

/*=====================================================================
| parameters definitions for SS7IFCREATE call
======================================================================*/
typedef enum {
      user_spare2 = 2,
      sccp_user   = 3, 
      tup_user    = 4,
      isup_user   = 5, 
      data_user1  = 6,
      data_user2  = 7,
      mtpt_user   = 8,
      user_spare9 = 9,
      user_spare10= 10,
      user_spare11= 11,
      user_spare12= 12,
      user_spare13= 13,
      user_spare14= 14,
      user_spare15= 15
} the_user_part;

typedef enum                     /* special connection request         */
{
     ss7_cnx_primary,            /* primary connection to Stack        */
     ss7_cnx_secondary           /* secondary connection to Stack      */
} ss7_mode;

typedef enum                     /* current proc type */
{
     NO_PROC = 0,
     ORDINARY_PROC,
     SYNCHRO_PROC,
     MONITOR_PROC
} ss7up_proc_type;

typedef struct {
   ss7up_proc_type        procType;
   char                   procIdent[MAXLEN_HOSTNAME+17]; /* 16 for %x pid */
} ss7_proc_desc;

typedef struct 
{
     the_user_part  SI;          /*                            for MTP */
     unsignedbool     restrict;         /* for old compatibility no more used */

     /* for SCCP */
     unsigned char ssn;          /*                  subsystem number */
     int           mode;         /*                   sccp mode       */

} ss7_service_parms ;

typedef struct                 /* additional connection information           */
{
     ss7_mode    cnx_mode;     /* connection mode (ss7_cnx_primary/secondary) */
     void       *add_info;     /* Used for Distributed ISUP feature. Must be 
                                  set to NULL except for sce type = 
                                  ss7_mtp_for_isup */
} ss7_cnx_info;

/*=====================================================================
|  Commands to control the ss7 interface: SS7IFCTRL
======================================================================*/

typedef enum {
      ss7_ipcsendsize,
      ss7_ipcrecvsize,
      ss7_low_t_time,
      ss7_high_t_time,
      ss7_block,
      ss7_dont_block,
      ss7_flush,
      ss7_flush_conditionnal,
      ss7_enable,
      ss7_disable
} ss7_ctrl_cmd;


/*=====================================================================
|  SCCP definitions
======================================================================*/

/* Sccp definitions are in separate include file sccpTypes.h and 
                                                 sccpOamDefs.h      */


/*=====================================================================
|  MTP definitions
======================================================================*/

/* Mtp definitions are in separate include file mtpTypes.h and 
                                                 mtpOamDefs.h      */


/*----------------------------------------------------------------------
| Functions prototypes
----------------------------------------------------------------------*/

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _WIN32
HP_DLL_IF int SS7_startup(ss7_startup_parameter * param);
HP_DLL_IF int SS7_cleanup( void );

HP_DLL_IF int SS7_postWaitForMultipleObjects (int      numHdl,
                                              HANDLE * hdlTab,
                                              int      wfmoRet,
                                              int      maxCnx,
                                              int *    numCnx,
                                              int * cnxIdsTab);

HP_DLL_IF int SS7_preWaitForMultipleObjects  (int * numHdl,
                                              int maxHdl,
                                              HANDLE *hdlTab,
                                              int * time);
/* this include should be removed when the ss7errno is removed
 * It is there only to define ss7_error_code
 */
#include "ss7errors.h"
HP_DLL_IF ss7_error_code ss7errno;

#endif  /* _WIN32 */

/* Common WIN32/HPUX */

HP_DLL_IF int SS7_ifclose (int cnxId);

HP_DLL_IF int SS7_ifcreate (char              *className,
                            ss7_sce_type      sceType,
                            ss7_service_parms *sceParms,
                            int               *cnxId);

HP_DLL_IF int SS7_ifctrl (int          cnxId,
                          ss7_ctrl_cmd command,
                          int          value);

#ifndef _WIN32

int SS7_ifcnxhandler (int    num_fd,
                      fd_set *read_mask,
                      fd_set *write_mask,
                      fd_set *exception_mask,
                      int    *num_cnx_active,
                      int    *cnx_active);

int SS7_ifselectmask (int    max_fd,
                      fd_set *read_mask,
                      fd_set *write_mask,
                      fd_set *exception_mask,
                      struct timeval **time);

int SS7_xifcreate (char              *className,
                   ss7_sce_type      sceType,
                   ss7_service_parms *sceParms,
                   void              *cnx_info,
                   int               *cnxId,
                   struct timeval    *timeout);


int SS7_specreate (char              *className,
                   ss7_process       process,
                   ss7_sce_type      sceType,
                   ss7_service_parms *sceParms,
                   int               *cnxId);

int SS7_xspecreate (char              *className,
                    ss7_process       process,
                    ss7_sce_type      sceType,
                    ss7_service_parms *sceParms,
                    void              *cnx_info,
                    int               *cnxId,
                    struct timeval    *timeout);

int SS7_tool_xifcreate ( char              * className,
                        ss7_sce_type         sceType,
                        ss7_service_parms  * sceParms,
                        void               * cnx_info,
                        int                * connection_id,
                        struct timeval     * timeout,
                        ss7up_proc_type      procType);

              
int SS7_tool_xspecreate ( char                * className,
                          ss7_process           process,
                          ss7_sce_type          sceType,
                          ss7_service_parms   * sceParms,
                          void                * cnx_info,
                          int                 * connection_id,
                          struct timeval      * timeout,
                          ss7up_proc_type       procType);

#endif /* _WIN32 */


#ifdef __cplusplus
}
#endif

#endif
