/***********************************************************
*
* $Header: /home/cvs/smsc/skt-smsc/include/ksig7/cfgCommon.h,v 1.1 2008/07/21 05:12:47 prowoni Exp $
*
* $Source: /home/cvs/smsc/skt-smsc/include/ksig7/cfgCommon.h,v $
* $Revision: 1.1 $
* $Locker:  $
* 
***********************************************************/
/*
// Filename: cfgCommon.g
// Creation date: 15/2/94
// Creator: BW
// Purpose:	Define config file strings used in global.conf file
*/

#ifndef CFGCOMMON_H
#define CFGCOMMON_H

/* Classnames */
#define CFGCOM_D_CLASS_SYSTEM		"System_Configuration"
#define CFGCOM_D_CLASS_FTC			"FT_Controller"
#define CFGCOM_D_CLASS_SS7			"SS7_Stack"
#define CFGCOM_D_CLASS_SLEE			"Slee"
#define CFGCOM_D_CLASS_DBMASTER                 "DbMaster"
#define CFGCOM_D_CLASS_EVENTHANDLER	"Event_Handler"
#define CFGCOM_D_CLASS_DISKLOGGER	"Disk_Logger_"
#define CFGCOM_D_CLASS_PRINTLOGGER	"Printer_Logger_"
#define CFGCOM_D_CLASS_ALARMHANDLER	"Alarm_Handler_"
#define CFGCOM_D_CLASS_TUNING	    "Performance_Tuning"
#define CFGCOM_D_CLASS_TYPEDB		"Type_DB"
#define CFGCOM_D_CLASS_MIB_AGENT	"Mib_Agent"
#define CFGCOM_D_CLASS_BUFFERS_PROCESS 	"Buffers_Process"

/* OC 1.2 entries. */
#define CFGCOM_C_MAXLEN_KEYNAME 	16
#define CFGCOM_D_CLASS_PATHS      	"Paths"
#define CFGCOM_D_KEY_CONFIG         "config"
#define CFGCOM_D_KEY_BINS           "bins"
#define CFGCOM_D_KEY_MISC           "misc"
#define CFGCOM_D_KEY_LOGS           "logs"
#define CFGCOM_D_KEY_TTL            "ttl"
#define CFGCOM_D_KEY_TMP            "tmp"

/* Common key names */
/* System ids */
#define CFGCOM_D_KEY_SYSTEMID		"SystemId_"
#define CFGCOM_D_KEY_PLATFORMID		"PlatformId"

/* Addressing info for all HA services */
#define CFGCOM_D_KEY_HOST_ADDRESS   "Host_Address_"
#define CFGCOM_D_KEY_HOST_ADDRESS_ALL "Host_Address_*"
#define CFGCOM_D_KEY_FT_PORT   		"FTPortName"
#define CFGCOM_D_KEY_FT2_PORT          "FT2PortName"

/* MIB Addressing for all MIB API processes */
#define CFGCOM_D_KEY_MIB_PORT		"MibPortName"
#define CFGCOM_D_KEY_MIB_AGT_PORT	"AgentPortName"

/* HB tuning for SO libs and FTC */
#define CFGCOM_D_KEY_FTC_HB_TIMEOUT		"FTCHeartbeatTimeout"
#define CFGCOM_D_KEY_FTC_HB_FREQ		"FTCHeartbeatFrequency"
#define CFGCOM_D_KEY_FTC_USE_EVENT		"UseEvent"
#define CFGCOM_D_KEY_PEER_HB_TIMEOUT	"PeerHeartbeatTimeout"
#define CFGCOM_D_KEY_PEER_HB_FREQ		"PeerHeartbeatFrequency"

/* HB channel tuning for SO lib */
#define CFGCOM_D_KEY_CLI_TIME		"ClientTime"
#define CFGCOM_D_KEY_SERVER_RETRY	"ServerRetryInterval"
/* Heart beat channel opened with peer ?
   Value = YES|NO, default = YES */ 
#define CFGCOM_D_KEY_HB_WITH_PEER   "HeartBeatWithPeer"	

/* Performance tuning for PFT/UFT libs */
#define CFGCOM_D_KEY_FT_BSIZE    "FT_BSize"
#define CFGCOM_D_KEY_PFT_NCHANS  "PFT_nChannels"
#define CFGCOM_D_KEY_PFT_SCHANS  "PFT_nSyncChannels"
#define CFGCOM_D_KEY_PFT_NMONS   "PFT_nMaxMonitors"
#define CFGCOM_D_KEY_PFT_BPERC   "PFT_Bufs_Per_Chan"
#define CFGCOM_D_KEY_UFT_NCHANS  "UFT_nChannels"
#define CFGCOM_D_KEY_UFT_BPERC   "UFT_Bufs_Per_Chan"

/* Synchronizing timers for Event handler */
#define CFGCOM_D_KEY_EVT_SYNCH_RETRY       "SynchonizeRetryInterval"
#define CFGCOM_D_KEY_EVT_SYNCH_COMPLETE    "SynchonizeCompletionInterval"

/* Type db/MibSchemaDB */
#define CFGCOM_D_KEY_TYPEDB_DIR	"TypeDB_Directory"

/* Other common config defines */
#define CFGCOM_D_SEPARATORS  " ,"        /* separators used on a config line */

#endif  /* CFGCOMMON_H */
