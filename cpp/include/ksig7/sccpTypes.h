/*=====================================================================
|                       
|                               sccpTypes.h
|                               -----------
|
| Common definitions used by both the SCCP object interface and the
| SCCP SS7 Application Programmatic Interface.
|
| $Source: /home/cvs/smsc/skt-smsc/include/ksig7/sccpTypes.h,v $
| $Revision: 1.1 $
| $Date: 2008/07/21 05:12:49 $
| HEWLETT PACKARD SS7
| (c) Copyright 1993, Hewlett-Packard Company, all rights reserved
|
======================================================================*/

#ifndef SS7_SCCP_TYPES_H
#define SS7_SCCP_TYPES_H

#ifdef _WIN32
#ifndef HP_DLL_IF
#  define HP_DLL_IF __declspec(dllimport)
#endif
#else
#  define HP_DLL_IF
#endif

typedef unsigned char   OCTET;   /* type of variables containing 1  BYTE*/
typedef unsigned char   BITS8;   /* type of variables containing 8  BITS*/
typedef unsigned short  BITshort;  /* type of variables containing 16 BITS*/
typedef unsigned int    BITS32;  /* type of variables containing 32 BITS*/

#ifndef Ubool_DEFINED
#define Ubool_DEFINED
typedef enum { no, yes } ubool;
#endif

#define TRANS_SEPARATOR 0x5f  /* '_' is the separator character */
#define TRANS_WILDCAR 0x5c   /* '\' is the wild character*/
#define MAXGTDIGLEN 30
/* Valid SSN range*/
#define MIN_USER        2
#define MAX_USER        255
/* Valid Point Code range */
#define MIN_SP      0
#ifdef ANSI88
#define MAX_SP      0xFFFFFF  /* ANSI point codes are 24 bits fields*/
#else
#define MAX_SP      0x3FFF    /* ITU-T point codes are 14 bits fields */
#define MAX_TTC_SP  0xFFFF    /* Japanese TTC point codes are 16 bits fields */
#define MAX_EXT_SP  0xFFFFFF  /* Chinese point codes are 24 bits fields */
#endif

/*-----------------------------------------------------------------------
   ENUMERATION TYPE for SCCP mode
   This enum is to be kept in sync with tcx_sccp_service defined 
   in TCAP_ext_ansi.h and TCAP_ext_ccitt.h
------------------------------------------------------------------------*/
typedef enum {

  SCCP_REGULAR_MODE =0,  /* old sccp behaviour (may signify CCITT Blue Book
                            or ANSI 88, depending on compilation option) */
  SCCP_ITU_WHITE,        /* Seg/Reas conformant to ITU-T White Book      */
/*GREhp13448*/
  SCCP_ANSI96_NOISNI =10 /* Seg/Reas conformant to ANSI 96 without ISNI  */

}SC_MODE ;

/* This structure is the private counterpart of 
   tc_private_service_parms for sccp private use. */
struct sc_prv_svc_parms {
   SC_MODE     SelMode;
};

/*-----------------------------------------------------------------------
   ENUMERATION TYPE for SCCP service class
------------------------------------------------------------------------*/
typedef enum {

  SCCP_CLASS_0 = 0,
  SCCP_CLASS_1

} SC_SERVICE_CLASS ;


/*-----------------------------------------------------------------------
 ENUMERATION TYPE for the Signalling Point Status
 (ref: Q.711 2.3.2.2.6)
------------------------------------------------------------------------*/
typedef enum
        {

         SC_inaccessible,
         SC_congested,
         SC_accessible,
         SC_restarting,
         SC_uncongested,
         SC_cnx_error

        }SC_SP_STATUS;

/*------------------------------------------------------------------------
 ENUMERATION TYPE for user status
 (ref: Q.711 2.3.2.2.3)
------------------------------------------------------------------------*/
typedef enum
        {

         SC_UIS,      /* user in-service */
         SC_UOS       /* user out-of-service */  

        }SC_USER_STATUS;

/*------------------------------------------------------------------------
 ENUMERATION TYPE for confirm status
------------------------------------------------------------------------*/
typedef enum
        {

         SC_granted_withdrawal,   /* "granted" N_COORD confirmation  */
         SC_denied_withdrawal,    /* "denied" N_COORD confirmation */  
         SC_no_peer               /* no backup is configured, gracefull */
                                  /* withdrawal is processed */
        }SC_CONFIRM_STATUS;

/*------------------------------------------------------------------------
 ENUMERATION TYPE for Subsystem multiplicity indicator
------------------------------------------------------------------------*/
typedef enum
        {
         
          multIndUnknown = 0,
          multIndSolitary = 1,
          multIndDuplicated = 2

        }SC_SMI;     /* coding of the subsystem multiplicity indicator */

/*------------------------------------------------------------------------
 ENUMERATION TYPE for the reason why a message was not able to be delivered
 to its final destination.
 (ref: Q.711 2.2.2.2.4 / Q. 713 3.12)
------------------------------------------------------------------------*/
typedef enum
  {
        
    SC_noTranslationNature = 0,         /* ITU-T BB, ANSI88 */
    SC_noTranslationSpecific = 1,       /* ITU-T BB, ANSI88 */
    SC_subsystemCongestion = 2,         /* ITU-T BB, ANSI88 */
    SC_subsystemFailure = 3,            /* ITU-T BB, ANSI88 */
    SC_unequippedUser = 4,              /* ITU-T BB, ANSI88 */
    SC_networkFailure = 5,              /* ITU-T BB, ANSI88 */
    SC_networkCongestion = 6,           /* ITU-T BB, ANSI88 */
    SC_unqualified = 7,                 /* ITU-T BB, ANSI88 */
    SC_errorInMsgTransport = 8,         /* ITU-T WB+96, ANSI96 (XUDTS only)*/
    SC_errorInLocalprocessing = 9,      /* ITU-T WB+96, ANSI96 (XUDTS only)*/
    SC_noDestReassembly = 10,           /* ITU-T WB+96, ANSI96 (XUDTS only)*/
    SC_sccpFailure = 11,                /* ITU-T WB+96, ANSI96 (XUDTS only)*/
    SC_hopCounterViolation = 12,        /* ITU-T 96, ANSI 96 */
    SC_segNotSupported = 13,            /* ITU-T 96 */
    SC_segFailure = 14,                 /* ITU-T 96 */
    SC_invalidISNIrouting = 249,        /* ANSI 96 */
    SC_unauthorizedMsg = 250,           /* ANSI 96 */
    SC_msgIncompatibility = 251,        /* ANSI 96 */
    SC_noISNIconsRouting = 252,         /* ANSI 96 */
    SC_redundantISNIconsRouting = 253,  /* ANSI 96 */
    SC_noISNIidentification = 254,      /* ANSI 96 */
    SC_noError = 256                    /* added: not defined in standards */

  }SC_RETURN_REASON;


/*-----------------------------------------------------------------------
 ENUMERATION TYPES for use with user global title structures
 enumeration defining types of global title in global title indicator
 (ref; CCITT Q.713 3.4.1)
-----------------------------------------------------------------------*/

typedef enum
        {

         SC_noGlobalTitle = 0,
         SC_gtType1 = 1, /* address-nature only, for CCITT  */
                         /* translation-type, numbering-plan and 
                            encode-scheme for ANSI */       
         SC_gtType2 = 2, /* translation-type only for CCITT and ANSI */
         SC_gtType3 = 3, /* translation-type, numbering-plan and 
                            encode-scheme for CCITT unused for ANSI */       
         SC_gtType4 = 4  /* address-nature, translation-type, 
                            numbering-plan and encode-scheme for CCITT
                            unused for ANSI */
        }SC_GT_INDICATOR;

/* enumeration defining Nature of Address parameter values for global titles
   Global title indicator = 0001 :
   (ref: CCITT Q.713  3.4.2.3.1)                                           
*/

typedef enum
        {

         SC_subscriberNo = 1,
         SC_nationalNo = 3,
         SC_internationalNo = 4

        }SC_ADDRESS_NATURE;

/* enumeration defining Bit 8 of octet 1 of the Nature of address indicator
*/

typedef enum
        {

         SC_evenNo = 0,
         SC_oddNo = 1

        }SC_ADDRESS_SIGNALS;


/* enumeration defining Translation Type parameter values for global titles
   Global title indicator = 0010 :
   (ref: CCITT Q.713  3.4.2.3.2)                                           
*/

typedef enum
        {

         SC_unused = 0,
         SC_internetwork_1 = 1,
         SC_networkSpecific_1 = 254

        }SC_TRANSLATION_TYPE;

/* enumeration defining Numbering Plan parameter values for global titles
   Global title indicator = 0011 :
   four bits (msb) of octet 2 of the Nature of address indicator
   (ref: CCITT Q.713  3.4.2.3.3)                                           
*/

typedef enum
        {

         SC_unknownNumbering = 0,
         SC_isdn = 1,
         SC_userdata = 3,
         SC_telex = 4,
         SC_maritimeMobile = 5,
         SC_landMobile = 6,
         SC_isdnMobile = 7

        }SC_NUMBERING_PLAN;

/* enumeration defining Encoding Scheme parameter values for global titles
   four bits (lsb) of octet 2 of the Nature of address indicator
   (ref: CCITT Q.713  3.4.2.3.3)
*/

typedef enum  
        {

         SC_unknownEncode = 0,
         SC_bcdOdd = 1,       /* BCD coding with odd  number of digits */
         SC_bcdEven = 2       /* BCD coding with even number of digits */

        }SC_ENCODING_SCHEME;


/*-----------------------------------------------------------------------
 STRUCTURE TYPES for global titles
---------------------------------------------------------------------- */
/* Structure containing a global title type 
   Global title indicator = 0100 :
   (ref: CCITT Q.713 3.4.2.3.4)
 NOTE: Parameters that are significat are defined by `gtIndicator`
*/

typedef struct
        {
         ubool                routOnGt;   /* TRUE if GT is used for routing */
         SC_GT_INDICATOR     gtIndicator; /* indicates type of global title*/
     /* nature = octet 1 of the Nature of address indicator */     
         SC_TRANSLATION_TYPE translation;  
     /* numbering + encoding = octet 2 of the Nature of address indicator */ 
         SC_NUMBERING_PLAN   numbering;    
         SC_ENCODING_SCHEME  encoding;     
     /* nature = octet 3 of the Nature of address indicator */     
         SC_ADDRESS_NATURE   nature;
         char                * digits;    /* address digits ? */
        }SC_GLOBAL_TITLE;


/* enumeration defining usage of point code provided or transmitted
   to user
*/

typedef enum  
        {

         SC_no      = 0,  /* No point code in SCCP part                    */
         SC_SccpUse = 1,  /* Point code is encoded in SCCP part            */
         SC_MtpUse  = 2   /* Point code is only used in MTP routing label  */
        }SC_PC_USAGE;

/*-----------------------------------------------------------------------
 Structure containing sccp address information
 ref: Q. 713  3.4
------------------------------------------------------------------------*/
typedef struct
        {

         SC_PC_USAGE  pointCodePresent;
         BITS32  pointCode;          
         ubool    ssnPresent;         /* TRUE if SSN contains data   */
         OCTET   SSN;                /* Subsystem number   */
         SC_GLOBAL_TITLE  *gt;       /* Pointer to global title (may be
                                      nil) nil means no global title. */ 
        }SC_ADDRESS;


/*=====================================================================
|  SCCP  API definitions
======================================================================*/

  typedef enum {sccp_unitdata_req, sccp_unitdata_ind, 
                sccp_coord_req, sccp_coord_ind, 
                sccp_coord_resp, sccp_coord_conf,
                sccp_pcstate_ind, 
                sccp_state_req, sccp_state_ind,
                sccp_notice_ind, sccp_notice_req,
                sccp_xunitdata_req, sccp_xunitdata_ind,
                sccp_xnotice_req, sccp_xnotice_ind
               } sccp_primitive;  

/*-------------------------------------------------------------------------
 Parameter types used for  N_UNITDATA request at the API interface      
 ref: TABLE 10/11 Q.711 
 -------------------------------------------------------------------------*/
typedef struct
        {

         SC_ADDRESS          calledAddress;
         SC_ADDRESS          callingAddress;
         unsigned char       sequenceControl; /* 0 -> no sequence (prot cl 0)*/
         ubool               returnOption;    /* true if return option req'd */
         int                 datalen;         /* length of data */
         unsigned char       * data;          /* string of data */

        }sccp_unitdata_req_parms;

/*-------------------------------------------------------------------------
 Parameter types used for  N_NOTICE request at the API interface        
 ref: TABLE 10/11 Q.711 
 -------------------------------------------------------------------------*/
typedef struct
        {

         SC_ADDRESS          calledAddress;
         SC_ADDRESS          callingAddress;
         SC_RETURN_REASON    returnReason;
         unsigned char       sequenceControl; /* 0 -> no sequence (prot cl 0)*/
         unsigned char       hopCounter; 
         int                 datalen;         /* length of data */
         unsigned char       * data;          /* string of data */

        }sccp_notice_req_parms;

/*-------------------------------------------------------------------------
 Parameters used for  N_UNITDATA and N_NOTICE indications at the API   
 interface.                                                              
 ref: TABLE 10 Q.711 
 -------------------------------------------------------------------------*/
typedef struct
        {

         SC_ADDRESS          calledAddress;
         SC_ADDRESS          callingAddress;
         SC_RETURN_REASON    returnReason;
         int                 datalen;         /* length of data */
         unsigned char       * data;          /* string of data */
         ubool               returnOption;    /* true if return option req'd */
         unsigned char       sequenceControl; /* 0 -> no sequence (prot cl 0)*/
         unsigned char       hopCounter; 

        }sccp_unitdata_ind_parms;

/*-------------------------------------------------------------------------
 Parameters used for  N_UNITDATA request and indication (ITU-T WB + 96)
 at the API interface (extended service).
 ref: Q.711 
 -------------------------------------------------------------------------*/
typedef struct
        {

         SC_ADDRESS          calledAddress;   /* called address  */
         SC_ADDRESS          callingAddress;  /* calling address */
         SC_SERVICE_CLASS    serviceClass;    /* required service class  */
         unsigned char       sequenceControl; /* sequence number */
         unsigned char       hopCounter;      /* reserved for LNP appli */
         ubool               returnOption;    /* true if return option req'd */
         ubool               XUDTOption;      /* 1 => force use of XUDT mess */
         ubool               SegmOption;      /* for future use */
         int                 importance;      /* priority [0-2] */
         int                 datalen;         /* length of data */
         unsigned char       * data;          /* string of data */

        }sccp_xunitdata_parms;


/*-------------------------------------------------------------------------
 Parameters used for  N_NOTICE indication (ITU-T WB + 96) and request (LNP
 feature) at the API interface.
 ref: Q.711 
 -------------------------------------------------------------------------*/
typedef struct
        {

         SC_ADDRESS          calledAddress;   /* called address  */
         SC_ADDRESS          callingAddress;  /* calling address */
         SC_RETURN_REASON    returnReason;    /* reason for return */
         unsigned char       sequenceControl; /* reserved for LNP appli */
         unsigned char       hopCounter;      /* reserved for LNP appli */

         /* InSequence field : yes => sls is derived from sequenceControl */
         /*                    no  => sequenceControl not significant     */
         ubool               InSequence;      /* reserved for LNP appli   */

         /* XUDTSOption field : yes => for use of XUDTS message           */
         ubool               XUDTSOption;     /* reserved for LNP appli   */

         ubool               SegmOption;      /* for future use */
         int                 importance;      /* priority [0-2] */
         int                 datalen;         /* length of data */
         unsigned char       * data;          /* string of data */

        }sccp_xnotice_parms;

/*-------------------------------------------------------------------------
   Parameters used for  N_STATE requests/indications.
   Sccp management primitives.
   ref: TABLE 14 and 15 of Q.711
 -------------------------------------------------------------------------*/

 typedef struct  
   { 

     SC_USER_STATUS     userStatus;
     OCTET              affectedSSN;  /* For indication only */
     BITS32             associatedPC; /* For indication only */                 
     SC_SMI             multInd;   /* multiplicity indicator (ind only) */

   }sccp_state_parms;

/*-------------------------------------------------------------------------
   Parameters used for   N_PCSTATE indications.
 -------------------------------------------------------------------------*/
 typedef struct 
   {

     BITS32             affectedDPC;
     SC_SP_STATUS       spStatus;

   }sccp_pcstate_parms;


/*-----------------------------------------------------------------------
   Parameters used for  N_COORD requests/confirmations/indications/reponses.
   Sccp management primitives.
   ref: TABLE 13 of Q.711
 -------------------------------------------------------------------------*/

 typedef struct  
   { 

     SC_CONFIRM_STATUS  coordStatus;
     OCTET              affectedSSN;  /* For indication only */
     BITS32             associatedPC; /* For indication only */                 
     SC_SMI             multInd;   /* multiplicity indicator (ind only) */

   }sccp_coord_parms;


/*=============================== PROTOTYPING ==============================*/

#ifndef NOPROTOTYPE
#ifdef __cplusplus
extern "C" {
#endif
   
HP_DLL_IF int SCCP_send ( int                connection_id,
                struct timeval     * time,
                sccp_primitive     primitive,
                char               * parms );


HP_DLL_IF int SCCP_recv ( int             connection_id,
                struct timeval  * time,
                sccp_primitive  * primitive,
                char            * parms );



#ifdef __cplusplus
}
#endif
#endif

#endif    /* SS7_SCCP_TYPES_H */
