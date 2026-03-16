/***********************************************************
*
* $Header: /home/cvs/smsc/skt-smsc/include/ksig7/TCAP_ansi.h,v 1.1 2008/07/21 05:12:49 prowoni Exp $
*
* $Source: /home/cvs/smsc/skt-smsc/include/ksig7/TCAP_ansi.h,v $
* $Revision: 1.1 $
* $Locker:  $
* 
***********************************************************/
/*
  ======================================================================
 |                          TCAP API include file                       |
 | 		                version ANSI                            |
 |                                                                      |
 |                                                                      |
 | HEWLETT PACKARD SS7                                                  |
 | (c) Copyright 1994, Hewlett-Packard Company, all rights reserved     |
 |                                                                      |
  ======================================================================
*/

#ifndef TCAP_ANSI_H
#define TCAP_ANSI_H

#ifndef _WIN32
#       include <time.h>
#       include <sys/types.h>
#endif /* _WIN32 */
#include "TCAP_common.h"

/*----------------------------------------------------------------------
 |                       COMPONENT types                               |   
 ----------------------------------------------------------------------*/

typedef enum {    

               TC_INVOKE_L, 
               TC_INVOKE_NL,
               TC_RESULT_L, 
               TC_RESULT_NL,
               TC_U_ERROR,
               TC_L_CANCEL, 
               TC_U_CANCEL,
               TC_L_REJECT, 
               TC_R_REJECT, 
               TC_U_REJECT

             } tc_component_type;



/*~~~~~~~~~~~~~~~~~~~~~ PARAMETER types and structure ~~~~~~~~~~~~~~~~~~*/
/*
HP reserved comment
@if not $_hp_reserved_t
*/

typedef enum { SET_TYPE, SEQUENCE_TYPE } tc_parm_type;

typedef struct tc_parameter_struct
           { 
            tc_parm_type      type;
            int               length;
            unsigned char     datas[MAX_PARAMETER_LEN];

           } tc_parameter;

/*
@endif
 */

/*~~~~~~~~~~~~~~~~ OPERATION / ERROR  types and structure ~~~~~~~~~~~~~~*/

typedef enum { NATIONAL, PRIVATE } tc_type;

typedef struct tc_error_struct
           {
            tc_type         tag;
            unsigned char   code;

           } tc_error;

typedef struct tc_operation_struct
           {
            tc_type         tag;
            unsigned char   op_family;
            unsigned char   op_specifier;

           } tc_operation;

/*~~~~~~~~~~~~~~~~~~~~~~~~~~ REJECT types ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

typedef enum {
 
              GENERAL, 
              INVOKE,
              RETURN_RESULT,
              RETURN_ERROR,
              TRANSACTION_PORTION

             } tc_pb_identifier;

/*~~~~~~~~~~~~~~~~~~~~~~~~~~ REJECT causes ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

typedef enum {

              UNRECOG_COMPONENT, 
              INCORRECT_COMPONENT_PORTION,
              BADLY_STRUCTURED_COMPONENT_PORTION,
              DUPLICATE_INVOKE_ID,
              UNRECOGNIZED_OPERATION_CODE,
              INCORRECT_PARAMETER,
              UNRECOGNIZED_CORRELATION_ID,
              UNEXPECTED_RETURN_RESULT,
              UNEXPECTED_RETURN_ERROR,
              UNRECOGNIZED_ERROR,
              UNEXPECTED_ERROR,
              UNRECOGNIZED_PACKAGE_TYPE,
              INCORRECT_TRANSACTION_PORTION,
              BADLY_STRUCTURED_TRANSACTION_PORTION,
              UNRECOGNIZED_TRANSACTION_ID

             } tc_pb_code;


/*~~~~~~~~~~~~~~~~~~~~~~~~~~ REJECT structure ~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

typedef struct tc_pcode_struct
           {
            tc_pb_identifier    identifier;
            tc_pb_code          code;

           } tc_pcode;


/*----------------------------------------------------------------------
 |                       COMPONENT structure                           |
 ----------------------------------------------------------------------*/

struct tc_component {

  tc_component_type  c_type;            /*                   component type */
#ifdef __cplusplus
  int                  op_class;        /* optionnal:  operation class 1..4 */
#else
  int                  class;           /*   default is no class = class 1  */
#endif
  int                  invoke_id;       /*                      -1 = absent */
  int                  correlation_id;  /*            optional: -1 = absent */
  tc_operation         operation;       /*                   operation code */
  tc_parameter         parameter;
  struct timeval       timer;           /*   optionnal: default is NO_TIMER */        
  tc_error             error;           /*          error (error component) */
  tc_pcode             problem_code;    /*  reject cause (reject component) */

  struct tc_component  * next_component;

};



/*~~~~~~~~~~~~~~~~~~~~~~~~~~ ADDRESS types ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

typedef enum { 
               DEFAULT_LPC,
               DPC,
               DPC_SSN,
               GT,
               GT_SSN,
               GT_DPC,
               GT_DPC_SSN,
               SSN,
               NO_TC_ADDRESS

             } tc_address_type;

/*----------------------------------------------------------------------
 |                    ADDRESS structure                                |
 ----------------------------------------------------------------------*/

typedef struct tc_address_struct {

            tc_address_type    type;             /* ETSI T/S 43-05 */
            int                pc_value;
            unsigned int       ssn;
            tc_global_title    gt;

          } tc_address ;

/*~~~~~~~~~~~~~~~~~~~~~~~~~~ P_ABORT causes ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

typedef enum { 

               TC_UNRECOG_PKG_TYPE,
               TC_INCORRECT_TRANS_PORTION,
               TC_BAD_STRUCTURED_TRANS_PORTION,
               TC_UNRECOG_TRANSACTION_ID,
               TC_PERM_TO_RELEASE_PROBLEM,
               TC_RESOURCE_UNAVAILABLE 

             } tc_p_abort_cause;


/*~~~~~~~~~~~~~~~~~~~~~~~~~~ U_ABORT struture ~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*
HP reserved comment
@if not $_hp_reserved_t
*/

typedef struct tc_u_abort_struct
           {
            int              length;
            unsigned char    datas[MAX_U_ABORT_LEN];

           } tc_u_abort;

/*
@endif
 */

/*----------------------------------------------------------------------
 |                     PRIMITIVE types                                 |
 ----------------------------------------------------------------------*/

typedef enum {

              TC_UNI,
              TC_QUERY_W_PERM,TC_QUERY_WO_PERM,
              TC_CONV_W_PERM,TC_CONV_WO_PERM,
              TC_RESPONSE,
              TC_U_ABORT, TC_P_ABORT,
              TC_NOTICE,
              MGT, SCCP_USER_STATUS, SCCP_PC_STATUS, 
              SCCP_N_COORD, SCCP_N_COORD_RES,
              NO_PRIMITIVE,
              SWITCH_STARTED, SWITCH_DONE

             } tc_primitive_type;


/* Part of tc_primitive stucture */

struct stat_p_abort                /*  For STAT_ABORT_RECEIVED/SENT */
{
  int value;                     /*             Number of P_ABORT */
  tc_p_abort_cause  p_abort;     /*        for this P_ABORT cause */
} ;

struct stat_reject                 /*     For STAT_REJECT_RECV/SENT */
{
  int value;                     /*              Number of REJECT */
  tc_pb_identifier p_identifier; /*          for this REJECT type */
  tc_pb_code  p_code;            /*          and this REJECT code */
} ;

struct user_load                   /*            For STAT_USER_LOAD */
{
  int user_id;                   /*    User identification number */
  int ssn;                       /*              Subsystem number */
  int load;                      /*           Number of TCmsg/sec */
} ;

struct tcStat
{
  tc_control_c      stat_type;         /* Statistic type identification */
#ifdef __LP64__
  int                 context;         /* request number identification */
#else
  void              * context;         /* request number identification */
#endif
  union
  {
    int            value;              /*         All statistics result */

    struct stat_p_abort abort;

    struct stat_reject reject;

    struct user_load u_load;
  } p;
}  ;

struct tc_pcstatus_struct
{
  tc_status_pc    status;             /*    Status of the destination */
  int             destConcerned;      /*  PC of concerned destination */
} ;

struct tc_ustatus_struct
{
  int             affectedSSN;         /*            Subsystem number */
  int             associatedPC;        /*       signalling point code */
  tc_status_user  status;              /*    status of concerned user */
} ;

/*~~~~~~~~   SCCP_N_COORD or SCCP_N_COORD_RES parameters ~~~~~~~~~~~~~~~*/

struct tc_ncoord_struct         /* for SCCP_N_COORD or SCCP_N_COORD_RES */
{
  int                affectedSSN;      /*            Subsystem number */
  int                associatedPC;     /*       signalling point code */
  tc_confirm_status  status;           /*   for SCCP_N_COORD_RES only,
                                            use to confirm withdraw */
  tc_smi            multInd;             /* for SCCP_N_COORD_RES only,
                                            Subsystem multiplicity indicator */
} ;

union tc_primitive_option_struct {

   /*~~~~~~~~~~~~~~~~~~~~~~~   TC_U_ABORT parameters   ~~~~~~~~~~~~~~~~~~~~~*/

   tc_u_abort   u_abort_cause;

   /*~~~~~~~~~~~~~~~~~~~~~~~   TC_END parameters   ~~~~~~~~~~~~~~~~~~~~~~~~~*/

   tc_termination_type   termination_type;

   /*~~~~~~~~~~~~~~~~~~~~~~~   TC_NOTICE parameters  ~~~~~~~~~~~~~~~~~~~~~~~*/

   tc_transport_return_reason   report_cause;

   /*~~~~~~~~~~~~~~~~~~~~~~~   TC_P_ABORT parameters  ~~~~~~~~~~~~~~~~~~~~~~*/

   tc_p_abort_cause     p_abort_cause;

   /*~~~~~~~~~~~~~~~~~~~~~~~   MGT parameters  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   struct tcStat tc_stat ;

    /*~~~~~~~~~~~~~~~~~~~   SCCP_PC_STATUS parameters  ~~~~~~~~~~~~~~~~~~~~~*/

    struct tc_pcstatus_struct tc_pcstatus;

    /*~~~~~~~~~~~~~~~~~~~  SCCP_USER_STATUS parameters ~~~~~~~~~~~~~~~~~~~~~*/

    struct tc_ustatus_struct tc_ustatus;

    /*~~~~~~~~   SCCP_N_COORD or SCCP_N_COORD_RES parameters ~~~~~~~~~~~~~~~*/

    struct tc_ncoord_struct tc_ncoord;

  } ;

/*----------------------------------------------------------------------
 |               ANSI  tc_primitive structure                          |
 |                                                                     |
 | Refer to tc_send man page for available parameters, mandatory and   |
 | optional                                                            |
 ----------------------------------------------------------------------*/

typedef struct tc_primitive_struct { 

 tc_primitive_type   p_type;               /*                Primitive type */

 tc_sce_quality      service_quality;      /*               Service quality */
 tc_address          d_address;            /*           destination address */
 tc_address          o_address;            /*           originating address */

 unsigned int        user_dialogue_id;     /* user reference of transaction */
 unsigned int        provider_dialogue_id; /* TCAP reference of transaction */

 union tc_primitive_option_struct tc_primitive_option;

} tc_primitive;


#endif /*TCAP_ANSI_H */
