/***********************************************************
*
* $Header: /home/cvs/smsc/skt-smsc/include/ksig7/TCAP_errors.h,v 1.1 2008/07/21 05:12:47 prowoni Exp $
*
* $Source: /home/cvs/smsc/skt-smsc/include/ksig7/TCAP_errors.h,v $
* $Revision: 1.1 $
* $Locker:  $
* 
***********************************************************/

/*=====================================================================
|
|                              TCAP_errors.h
|                              -------------
|                            TCAP API error codes
|
|
| HEWLETT PACKARD SS7
| (c) Copyright 1991, 1992, 1993 Hewlett-Packard Company, all rights reserved
|
======================================================================*/

#ifndef TCAP_ERRORS
#define TCAP_ERRORS

//#define tc_error_h_rcsid "$Source: /home/cvs/smsc/skt-smsc/include/ksig7/TCAP_errors.h,v $ $Revision: 1.1 $ $Date: 2008/07/21 05:12:47 $ ";
//static const char *tc_error_h_rcsid = "$Source: /home/cvs/smsc/skt-smsc/include/ksig7/TCAP_errors.h,v $ $Revision: 1.1 $ $Date: 2008/07/21 05:12:47 $ ";

/*
 * *****************************************************************
 * this file describes all the possible errors which can be returned
 * by a TCAP API call.
 * *****************************************************************
 */

typedef enum {
  
  TCE_NO_ERROR = 0,

  TCE_CNX_ID,	
  TCE_CONNECT_INIT,

  TCE_NO_CONFIG,
  TCE_BAD_CONFIG,

 
  TCE_CNX_CLOSED = 5,

  TCE_ILLEGAL_VALUE,
  TCE_NOT_IMPLEMENTED,
  TCE_MEMORY_ERROR,
  TCE_SEND_FULL,
  TCE_INTERNAL = 10 ,
  TCE_MAX_USERS,
  TCE_SYNTAX_ERROR,
  TCE_COMPONENT_ERROR,
  TCE_TOO_MANY_COMPONENTS,
  TCE_ILLEGAL_CALL = 15,
  TCE_WRONG_IDS,
  TCE_INVALID_SSN,
  TCE_API_BUSY,
  TCE_WRONG_APPLICATION_ID,
  TCE_WRONG_INSTANCE_ID,

  TCE_BAD_VERSION,
  TCE_NO_SSN,
  TCE_NO_SERVICE

} tc_error_code ;

/******************************************************************************
*
*  To get the error message associated with the error number,
*  use 'tc_error_text' external variable. 
*  For instance: tc_error_text [TCE_CONNECT_INIT] is a pointer to a string
*  which is the text of the error message. 
*
******************************************************************************/

#endif
