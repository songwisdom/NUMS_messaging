/* ===============================================================
*		MODULE NAME		: EMacro.h
*		PURPOSE			: Expand Macro.
*		INPUT ARGUMENT	: NONE
*		OUTPUT ARGUMENT	:
*		RETURN VALUE	: NONE
*		REVISION		:		BY			WHEN	 	REASON
*					  	 -------------  ------------  -----------
*							seoki		2001.11.05
================================================================== */

#ifndef __EMACRO_H__
#define __EMACRO_H__

#include <FLKBaseDef.h> // Including New System Definition.

#define SCHD_TIME_FMT		"YYYYMMDD.hhmmss"
#define ETIME_BEFORE_MODE	0
#define ETIME_NOW_MODE		1
#define ETIME_AFTER_MODE	2

#define COMMON_ERROR_MSG	"common lib error"

#define DBH_TO_SSMSREQ_IVK              0x01
#define MDBH_TO_SSMSREQ_IVK             0x01
#define SCHD_TO_SSMSREQ_IVK             0x01
#define RSMSREQ_RET_TO_SSMDPP_IVK       0x02
#define SCHD_TO_SSMDPPQ_IVK             0x02

#define SEND_SMSREQ_IVK                 0x01
#define SEND_SMDPP_IVK                  0x02

#define SMDPP_IVK_TCAP_TO_ASE           0x2000
#define SMDPP_RET_TCAP_TO_ASE           0x35
#define SMSREQ_RET_TCAP_TO_ASE          0x37
#define SMSREQ_IVK_TCAP_TO_ASE          0x37
#define SMSNOT_IVK_TCAP_TO_ASE          0x36
#define SMSNOT_RET_TCAP_TO_ASE          0x36

#define MAX_DATA_LENGTH                 1024

/*
 * shared memory keys for performance statistics
 */

/***  Tele Service Id definition ***/
#define     PPT     0x1001
#define     PMT     0x1002
#define     VMN     0x1003

#endif /* __EMACRO_H__ */
