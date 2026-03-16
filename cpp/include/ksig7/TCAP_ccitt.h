/***********************************************************
 *
 * $Header: /home/cvs/smsc/skt-smsc/include/ksig7/TCAP_ccitt.h,v 1.1 2008/07/21 05:12:49 prowoni Exp $
 *
 * $Source: /home/cvs/smsc/skt-smsc/include/ksig7/TCAP_ccitt.h,v $
 * $Revision: 1.1 $
 * $Locker:  $
 * 
 ***********************************************************/
/*
   ======================================================================
   |                          TCAP API include file                       |
   |                 version CCITT   Blue Book / White Book               |
   |                                                                      |
   |                                                                      |
   | HEWLETT PACKARD SS7                                                  |
   | (c) Copyright 1994, Hewlett-Packard Company, all rights reserved     |
   |                                                                      |
   ======================================================================
 */

#ifndef TCAP_CCITT_H
#define TCAP_CCITT_H

#ifndef _WIN32
#       include <time.h>
#       include <sys/types.h>
#endif /* _WIN32 */
#include "TCAP_common.h"

#define MAX_ERROR_LEN              100
#define MAX_OPERATION_LEN          100

/*----------------------------------------------------------------------
  |                       COMPONENT types                               |
  ----------------------------------------------------------------------*/

typedef enum {

	TC_INVOKE, 
	TC_RESULT_L, TC_RESULT_NL,
	TC_U_ERROR,
	TC_L_CANCEL, TC_U_CANCEL, 
	TC_L_REJECT, TC_R_REJECT, TC_U_REJECT 

} tc_component_type;



/*~~~~~~~~~~~~~~~~~~~~~ PARAMETER structure ~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 *  The tc_parameter_struct corresponds to the "parameter field" defined
 *  by the CCITT blue book. Parameter type tag shall be any ASN.1 tag,
 *  depending on the type of the parameter supplied.  The field "datas"
 *  of the "tc_parameter_struct" must contains the parameters, the length
 *  already encoded, and the tag.
 *
 */

/*
   HP reserved comment
   @if not $_hp_reserved_t
 */

typedef struct tc_parameter_struct
{ 
	int              length;
	unsigned char    datas[MAX_PARAMETER_LEN];

} tc_parameter;

/*~~~~~~~~~~~~~~~~ OPERATION / ERROR  types and structure ~~~~~~~~~~~~~~*/

typedef enum { LOCAL_TYPE, GLOBAL_TYPE } tc_tag;

typedef struct tc_error_struct
{
	tc_tag          tag;
	int             length;
	unsigned char   datas[MAX_ERROR_LEN];

} tc_error;

typedef struct tc_operation_struct
{
	tc_tag          tag;
	int             length;
	unsigned char   datas[MAX_OPERATION_LEN];

} tc_operation;

/*
   @endif
 */

/*~~~~~~~~~~~~~~~~~~~~~~~~~~ REJECT types ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

typedef enum {
	
	GENERAL,
	INVOKE,
	RETURN_RESULT,
	RETURN_ERROR

} tc_pb_identifier;


/*~~~~~~~~~~~~~~~~~~~~~~~~~~ REJECT causes ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

typedef enum {

	UNRECOG_COMPONENT, 
	MISTYPED_COMPONENT, 
	BADLY_STRUCTURED_COMPONENT, 
	DUPLICATE_INVOKE_ID,
	UNRECOGNIZED_OPERATION, 
	MISTYPED_PARAMETER, 
	RESOURCE_LIMITATION, 
	INITIATING_RELEASE, 
	UNRECOGNIZED_LINKED_ID, 
	LINKED_RESPONSE_UNEXPECTED,
	UNEXPECTED_LINKED_OPERATION,
	UNRECOGNIZED_INVOKE_ID,
	RETURN_RESULT_UNEXPECTED, 
	RETURN_ERROR_UNEXPECTED,
	UNRECOGNIZED_ERROR, 
	UNEXPECTED_ERROR 

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

	tc_component_type    c_type;           /*               component type */
#ifdef __cplusplus
	int                  op_class;         /*        operation class  1..4 */
#else
	int                  class;            /*        operation class  1..4 */
#endif
	int                  invoke_id;        /*                   -1= absent */
	int                  linked_id;        /*         optional: -1= absent */
	tc_operation         operation;        /*               operation code */
	tc_parameter         parameter;   
	struct timeval       timer;            /*      operation timeout value */
	tc_error             error;            /* error code (error component) */
	tc_pcode             problem_code;     /*                  reject code */

	struct tc_component  * next_component;

};


/*~~~~~~~~~~~~~~~~~~~~~~~~~~ ADDRESS types ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

typedef enum { 
	NO_TC_ADDRESS, 
	DEFAULT_LPC,
	DPC, 
	DPC_SSN, 
	GT, 
	GT_SSN,
	GT_DPC,
	GT_DPC_SSN,
	SSN  
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


#ifdef TCAP_WHITE   /**** USED ONLY FOR TCAP CCITT WHITE BOOK ****/

typedef enum { 
	TC_UNRECOG_MSG_TYPE,
	TC_UNRECOG_TRANSACTION_ID,
	TC_BAD_FMT_TRANS_PORTION,
	TC_INCORRECT_TRANS_PORTION,
	TC_RESOURCE_LIMITATION ,
	TC_ABNORMAL_DIALOGUE,
	TC_NO_COMMON_DIALOGUE_PORTION
} tc_p_abort_cause;

#else

typedef enum { 
	TC_UNRECOG_MSG_TYPE,
	TC_UNRECOG_TRANSACTION_ID,
	TC_BAD_FMT_TRANS_PORTION,
	TC_INCORRECT_TRANS_PORTION,
	TC_RESOURCE_LIMITATION
} tc_p_abort_cause;

#endif



/*~~~~~~~~~~~~~~~~~~~~~~~~~~ U_ABORT struture ~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*
   HP reserved comment
   @if not $_hp_reserved_t
 */

typedef struct { 

	/* Set length = 0 when abort information is not specified */
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
	TC_BEGIN,
	TC_CONTINUE,
	TC_END, 
	TC_U_ABORT, TC_P_ABORT, 
	TC_NOTICE, 
	MGT, SCCP_USER_STATUS, SCCP_PC_STATUS, 
	SCCP_N_COORD, SCCP_N_COORD_RES,
	NO_PRIMITIVE, 
	SWITCH_STARTED, SWITCH_DONE

} tc_primitive_type;



/*----------------------------------------------------------------------
  |                  DIALOGUE PORTION Structures                        |
  ----------------------------------------------------------------------*/

#ifdef TCAP_WHITE   /**** USED ONLY FOR TCAP CCITT WHITE BOOK ****/


typedef struct 
{
	int      length;
	char     data [MAX_FIELD];

} tc_data;

typedef enum 
{
	AC_name_not_supported,
	user_specific

} tc_abort_reason;


typedef struct {

	/* indicates that dialogue information are present in the primitive   */

	TC_BOOL          dlg_info_present;

	/* identifier of the application context ( set of A.S.E ) proposed    */
	/* by the dialogue initiator or by the dialogue responder.            */
	/* length = 0 means absent.                                           */

	tc_data          application_context_name; 

	/* information which can be exchanged between TC_users independently  */
	/* from the remote operation service (length = 0 means absent).       */
	/* user_information contents is completely transparent to HP SS7      */
	/* TCAP, but should be formatted according to the table 49 of the     */
	/* Q773 CCITT WHITE BOOK recommendation.                              */

	tc_data          user_information;

	/* indicates wether a dialogue is aborted because the received appli- */
	/* cation context name is not supported and no alternative can be     */
	/* proposed or because of any other user problem.                     */

	tc_abort_reason  abort_reason;  

} tc_dialog_portion;

#endif



/*----------------------------------------------------------------------
  |      Specifiques MACROS for access to the white book parameters     |
  ----------------------------------------------------------------------*/

#ifdef TCAP_WHITE

#define TC_P_DLG_INFO_PRESENT(aStruct) \
	((aStruct)->dialog_portion.dlg_info_present)
#define TC_P_DLG_PORTION_APP_CONTEXT(aStruct) \
	((aStruct)->dialog_portion.application_context_name)
#define TC_P_DLG_PORTION_USER_INFO(aStruct) \
	((aStruct)->dialog_portion.user_information)
#define TC_P_DLG_PORTION_ABORT_REASON(aStruct) \
	((aStruct)->dialog_portion.abort_reason)

#endif

/* STATISTICS structures */

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

union tcStat_content
{
	int            value;              /*         All statistics result */
	struct stat_p_abort abort;
	struct stat_reject reject;
	struct user_load  u_load;
} ;

struct tcStat 
{
	tc_control_c      stat_type;         /* Statistic type identification */
#ifdef __LP64__
	int               context;           /* request number identification */
#else
	void              * context;         /* request number identification */
#endif
	union tcStat_content p ;
} ;


/*~~~~~~~~~~~~~~~~~~~   SCCP_PC_STATUS parameters  ~~~~~~~~~~~~~~~~~~~~~*/

struct tc_pcstatus_struct
{
	tc_status_pc    status;	            /*    Status of the destination */
	int             destConcerned;	    /*  PC of concerned destination */
} ;

/*~~~~~~~~~~~~~~~~~~~  SCCP_USER_STATUS parameters ~~~~~~~~~~~~~~~~~~~~~*/

struct tc_ustatus_struct
{
	int             affectedSSN;         /*            Subsystem number */
	int             associatedPC;        /*       signalling point code */
	tc_status_user  status;	             /*    status of concerned user */
} ;

/*~~~~~~~~   SCCP_N_COORD or SCCP_N_COORD_RES parameters ~~~~~~~~~~~~~~~*/

struct tc_ncoord_struct         /* for SCCP_N_COORD or SCCP_N_COORD_RES */
{        
	int                affectedSSN;      /*            Subsystem number */
	int                associatedPC;     /*       signalling point code */
	tc_confirm_status  status;	     /*   for SCCP_N_COORD_RES only,
										  use to confirm withdraw */
	tc_smi		multInd;     /* for SCCP_N_COORD_RES only,
								Subsystem multiplicity indicator */
} ;

union tc_primitive_option_struct {

	/*~~~~~~~~~~~~~~~~~~~~~~~   TC_U_ABORT parameters   ~~~~~~~~~~~~~~~~~~~~~*/

	tc_u_abort	u_abort_cause;	

	/*~~~~~~~~~~~~~~~~~~~~~~~   TC_END parameters   ~~~~~~~~~~~~~~~~~~~~~~~~~*/

	tc_termination_type   termination_type;

	/*~~~~~~~~~~~~~~~~~~~~~~~   TC_NOTICE parameters  ~~~~~~~~~~~~~~~~~~~~~~~*/

	tc_transport_return_reason   report_cause;

	/*~~~~~~~~~~~~~~~~~~~~~~~   TC_P_ABORT parameters  ~~~~~~~~~~~~~~~~~~~~~~*/

	tc_p_abort_cause 	p_abort_cause;

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
  |                     tc_primitive structure                          |
  |                                                                     |
  | Refer to tc_send man page for available parameters, mandatory and   |
  | optional                                                            |
  ----------------------------------------------------------------------*/


typedef struct tc_primitive_struct {  

	tc_primitive_type   p_type;	           /*                Primitive type */

	tc_sce_quality	     service_quality;      /*               Service quality */
	tc_address          d_address;            /*           destination address */
	tc_address          o_address;            /*           originating address */

	unsigned int        user_dialogue_id;     /* user reference of transaction */

	/* TCAP reference of transaction, often used as originating transaction id */
	unsigned int        provider_dialogue_id; 

#ifdef TCAP_WHITE
	tc_dialog_portion  dialog_portion;   /* Application context and user info */
#endif

	union tc_primitive_option_struct  tc_primitive_option;
} tc_primitive;


#endif /* TCAP_CCITT_H */
