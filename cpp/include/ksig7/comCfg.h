/**begin_is**/
/*---------------------------------------------------------------------------
|
|                               MODULE  libComCfg
|                               ---------------------
|
|
|  PURPOSE:  Predecs for common parsing config file routines
|
|  NOTES and CAUTIONS:
|		
|
|____________________________________________________________________________
|
|
|  AUTHOR: rk
|  DATE:   Fri May 13 13:41:15 IST 1994
|----------------------------------------------------------------------------
|  Revision: $Header: /home/cvs/smsc/skt-smsc/include/ksig7/comCfg.h,v 1.1 2008/07/21 05:12:47 prowoni Exp $
|            $Source: /home/cvs/smsc/skt-smsc/include/ksig7/comCfg.h,v $
|            $Revision: 1.1 $
|            $Locker:  $
|----------------------------------------------------------------------------
|  HISTORY:
|
|  Date       Modification Location              Summary              Name
|  ----       ---------------------              -------              ----
|--------------------------------------------------------------------------*/
/**end_is**/

/* 
 * @HB : For WindowsNT DLL implementation, the function declarations are moved to
 *       the DLL header file CfgAndTtl.h.
 *       Unfortionately, CfgAndTtl.h is included when ainCommon.h is included.
 *       For this reason, the typedef, which is used in the function declaration,
 *       has also been moved.
 */

#ifndef	COMCFG_H
#define	COMCFG_H


#include <ainCommon.h>
#include <cfgCommon.h>
#ifdef COCA_WIN32
#include <windows.h>
#endif /* COCA_WIN32 */

#define COM_C_MAXLEN_PATHNAME	127
#define COM_C_MAXLEN_VALUE 127


#ifdef __cplusplus
extern "C" {
#endif

    /* Structure used by helper function to parse config file info for ha service addressing */
#define MAX_SERVICEINFO_HOSTS   2 	/* Move to .h when we need more than 2 */
typedef struct {
  int nHosts;         /* number of host found */
  int myHost;         /* index of myHost in the array */
  char *hosts[MAX_SERVICEINFO_HOSTS];     /* list of host found */
  char *services[MAX_SERVICEINFO_HOSTS];  /* list of service found (linked to list of host) */
  char *hb_service;   /* legacy service port */
  char *mib_service;  /* legacy mib_service port */ 
    
} COM_T_service_info;
    

    /* Saturn FDR type */
typedef struct {
  int nHosts;                                  /* number of host found */
  int myHost;                                  /* index of myHost in the array */
  int peerHost;                                /* index of the peer host in  the array */
  char hosts[MAX_SERVICEINFO_HOSTS][MAXLEN_HOSTNAME];              /* list of host found */
  char services[MAX_SERVICEINFO_HOSTS][MAXLEN_SERVICENAME];        /* SO server service */
  char SOSO_services[MAX_SERVICEINFO_HOSTS][MAXLEN_SERVICENAME];   /* SO-SO service for subftc */
  char hb_services[MAX_SERVICEINFO_HOSTS][MAXLEN_SERVICENAME];     /* ftc-ftc hb service */
  char mib_services[MAX_SERVICEINFO_HOSTS][MAXLEN_SERVICENAME];    /* mib server service */
  int systemIds[MAX_SERVICEINFO_HOSTS];                            /* system id */
} COM_T_xservice_info;

int COM_gethostname(char* host , const int P_len);    
extern COM_T_service_info *COM_get_service_info(int cfg_id, char *classname);
int COM_xget_service_info(COM_T_xservice_info *si, int cfg_id, char *classname); 
extern int COM_get_system_id();
extern int COM_get_system_id_host(char *host);
extern int COM_get_platform_id();
extern char * COM_get_mib_port_name(char *classname);
extern char * COM_xget_mib_port_name(int P_sysId, char *classname) ; 
extern char *COM_get_filepath(int cfgId, char *classname, char *key);
extern char *COM_get_TB_dir();
extern char *COM_find_right_mib_service(int systemId, char *entityClassName);
extern char *COM_whichPathFor( char *P_keyName );

#define COM_XGET_MAXVALUES 10
extern int COM_xget_service_info(COM_T_xservice_info *si, int cfg_id, char *classname);
    
#ifdef COCA_WIN32
/* Library entry point */
extern bool libComCfg_DllMain (HANDLE hInst, 
                            ULONG ul_reason_for_call,
                            LPVOID lpReserved);
#endif


#ifdef __cplusplus
}   /* extern "C" */
#endif


#endif /* COMCFG_H */
