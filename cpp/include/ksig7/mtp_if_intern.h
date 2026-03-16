/***********************************************************
*
* $Header: /home/cvs/smsc/skt-smsc/include/ksig7/mtp_if_intern.h,v 1.1 2008/07/21 05:12:50 prowoni Exp $
*
* $Source: /home/cvs/smsc/skt-smsc/include/ksig7/mtp_if_intern.h,v $
* $Revision: 1.1 $
* $Locker:  $
* 
***********************************************************/
 
#ifndef MTP_IF_INTERN_H
#define MTP_IF_INTERN_H

#include "ainCommon.h"

/*----------------------------------------------------------------------------
The  MTPL3_ctrl_cmd commands specify the type of action (add or remove) to 
perform.
Either a list of dpcs can be set under the control of isup instance or removed.
As well the list of dpcs is stored in or removed from the DataStore 
-----------------------------------------------------------------------------*/
typedef enum
{
  ADD_DPCS_TO_ROUTER,        /* add list of dpcs    */
  REMOVE_DPCS_FROM_ROUTER    /* remove list of dpcs */
} MTPL3_ctrl_cmd;

/*-----------------------------------------------------------------------------
  MTPL3_ctrl_paramType specifies the type of data
-----------------------------------------------------------------------------*/
typedef enum
{
  DPC_LIST                /* data type is a list of dpcs*/
} MTPL3_ctrl_paramType;

/*----------------------------------------------------------------------------
  MTPL3_ctrl_paramType specifies the struct to be used
---------------------------------------------------------------------------- */
typedef enum
{
  dist_isup_data,          /* MTP3_dataStruct is considered */
  dist_isup_control        /* MTP3_ctrlStruct is considered */
} mtpPduType;

/*----------------------------------------------------------------------------
  statusDataStore report all the possible statement of the actions performed 
  on DataStore
-----------------------------------------------------------------------------*/

typedef enum
{
  OK_INIT,             /* table created successfully in DataStore          */
  NOT_OK_INIT,         /* issue occurs while creating table in DataStore: 
                          table not created                                */
  TABLE_LOCKED,        /* table is locked, cannot access it                */ 
  FOUND,               /* application Id related to the dpc found          */ 
  NOT_FOUND,           /* application Id related to the dpc not found      */ 
  OK_ADD,              /* dpcs added successfully                          */ 
  NOT_OK_ADD,          /* issue occurs while adding dpc in DataStore: 
                       dpc not added                                       */ 
  OK_REMOVE,           /* dpcs removed successfully                        */ 
  NOT_OK_REMOVE,       /* issue occurs while removing dpc in DataStore: 
                          dpc not removed                                  */ 
  OK_DESTROY,          /* table destroyed successfully in DataStore        */ 
  NOT_OK_DESTROY,      /* issue occurs while destroying table in DataStore */ 
  OK_REMOVE_APPID,     /* application Id remove ok from DataStore          */ 
  NOT_OK_REMOVE_APPID  /* remove pllication Id from DataStore failed       */ 
} statusDataStore;


/*----------------------------------------------------------------------------
 RouterStatus contains information relating the routing actions. status
 contains the statement of the action performed and dpcList contains either the
 list of dpcs if the action failed or NULL if the action succeeded. 
-----------------------------------------------------------------------------*/
typedef struct
{
  statusDataStore    status;        /* action fails? succeeds?          */
  char *             dpcList;       /* possible list of dpcs an action 
                                       failed for                       */
  COM_T_uint32       dpcListLength; /* length of the possible dpc list  */
} routerStatus;

/*----------------------------------------------------------------------------
 Contains the application Id
-----------------------------------------------------------------------------*/
typedef struct
{
   COM_T_uint16 applicationId;       /* application Id value related to an 
                                        isup instance group (primary and 
                                        secondary)                       */
} ss7_additional_cnx_info;

/*----------------------------------------------------------------------------
  MTPL3_ctrl is used by isup instances to add and remove param.
    - cnxId: connection Id
    - cmd: command add or remove
    - paramType: parameter type (for instance list of dpc) 
    - paramLength: parameter length
    - param: parameter to be treated (list of dpc)
-----------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif

int MTPL3_ctrl(int cnxId, MTPL3_ctrl_cmd cmd, MTPL3_ctrl_paramType paramType, int paramLength, char *param);

#ifdef __cplusplus
}
#endif

#endif
