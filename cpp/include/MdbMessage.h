#ifndef __MDBMESSAGE_H__
#define	__MDBMESSAGE_H__

#include "FLKBaseDef.h"

#define RESULT                          0x01000000
#define NO_RESULT                       0x02000000

/* Request EventName */
#define  DBMS_CONNECT                   0x01
#define  MULTI_QUERY                    0x09

#define  SELECT                 0x10
#define  SELECT_SEQ             0x11
#define  SELECT_NEQ             0x12
#define  SELECT_EX3             0x13
#define	 SELECT_EX1             0x14
#define  SELECT_EX2             0x15

#define  UPDATE             	0x20
#define  UPDATE_SEQ             0x21
#define  UPDATE_EX              0x22
#define  UPDATE_EO              0x23
#define  UPDATE_EX2             0x24
#define  UPDATE_EX3             0x25
#define  UPDATE_ATTR            0x26
#define  UPDATE_ATTR_EX1        0x27
#define  UPDATE_ATTR_EX2        0x28
#define  UPDATE_ATTR_EX3        0x29

#define  DELETE                 0x30
#define  DELETE_SEQ             0x31
#define  DELETE_EX1             0x32
#define  DELETE_EX2             0x33

#define  INSERT             0x40

#define  COND_CURSOR                    0x50
#define  FIRST_CURSOR                   0x51
#define  LAST_CURSOR                    0x52
#define  NEXT_CURSOR                    0x53
#define  PREV_CURSOR                    0x54
#define  FREE_CURSOR                    0x55

#define  COUNT                  0x60

#define  RDIC               0x80
#define  RDIR               0x81

#define  QUIT               0x99


/* Reply EventName */
#define  CONNECT_ACK                    0x900
#define  MULTI_ACK                      0x908

#define MAX_QUERY_MSG       100

#define	DB_DML_SUCCESS		1
#define SME_ADDRESS         1
typedef struct {
	u_char   ucMesgId[4];
	u_char   ucSubmittTime[7];
} stMsgList;

#define  SELECT_ACK         0x910
#define  UPDATE_ACK         0x920
#define  DELETE_ACK         0x930
#define  INSERT_ACK         0x940
#define  CURSOR_ACK         0x950
#define  COUNT_ACK          0x960
#define  RDIC_ACK           0x980
#define  RDIR_ACK           0x981
#define  QUIT_ACK           0x989

#define  REQUEST_FAIL       0x990
#define  TIME_OUT           0x999
#define EXTRA3_SELECT       0x00000100

#define SEND_MESG_DUMP              0x10000000
#define  RECV_MESG_DUMP             0x20000000
/* limit variable */
//#define  QUERY_UNIT_REQ_SIZE        610
//#define  QUERY_UNIT_ACK_SIZE        610

/*
 *	Increase size of ATT/TUPLE
 *	ATT_PER_TABLE	160 -> 180
 *	TUPLE_SIZE		620 -> 640
 *	2006.09.14 R.142 by bjlee
 */
/* baram MAX_ATT_PER_QP */
//#define MAX_ATT_PER_QP              12
#define MAX_ATT_PER_QP              36 // R910 PKG 24 -> 36 (SMPP MESSAGE ID)
// #define MAX_ATT_PER_TABLE           180
#define MAX_ATT_PER_TABLE           280 //R100 pkg 250 -> 270 -> 280
/* FR220 : Contentsize 181*/
//#define MAX_TUPLE_SIZE         	670
//#define MAX_TUPLE_SIZE         		730

/*
#define MAX_TUPLE_SIZE         		780 // FR530 pkg 770 -> 780
#define  QUERY_UNIT_REQ_SIZE        610
#define MULTI_COUNT                 3
#define  QUERY_UNIT_ACK_SIZE        610
*/
#define MAX_TUPLE_SIZE         		2048 // R110 1024 -> 2048
#define MAX_STAT_TUPLE_SIZE   		100 // SR124 BMT 1000 -> 100
#define  QUERY_UNIT_REQ_SIZE        1024
#define MULTI_COUNT                 3
#define  QUERY_UNIT_ACK_SIZE       1024
#define MAX_STAT_QCNT 			16 //SR124 BMT

typedef struct _sttDBMSKey {
	key_t MQKeyDBMSQ;
	key_t MQKeyDBMSQ_RESULT;
	key_t MQKeyDBMSQ_BACKUP;

	//SR124 BMT
	key_t MQKeyDBMSQ_STAT[MAX_STAT_QCNT];

	key_t SMKeyDBMS;
	key_t SMKeyDBKEY;
	key_t SMKeyINDEX;

	key_t SMKeySemBase;
} sttDBMSKey;

/* IDentification number */
typedef struct {
	int		srcQid ;
	int		dstQid ;

	//SR124 BMT
	int  	statQid[MAX_STAT_QCNT];
	int 	send_stat_qid;

	long		type;

	int			connect_flag;
//	int			result_flag;
	sttDBMSKey	stDBMSKey;
} IDs ;


/* Request Data Structure */
typedef struct {
	int   table_id;
	int   att_id;
	char  att_val[MAX_ATT_PER_QP];
	int   tup_id;
} Select;

typedef struct {
	int   	  table_id;
	int       att_id;
	char      att_val[MAX_ATT_PER_QP];
	int       tup_id;
	int       update_cnt;
	char      update_list[MAX_ATT_PER_TABLE];
	char      tuple[MAX_TUPLE_SIZE];
} Update;

typedef struct {
	int       table_id;
	int       att_id;
	char      att_val[MAX_ATT_PER_QP];
	int       tup_id;
} Delete;

typedef struct {
	int       table_id;
	char      tuple[MAX_TUPLE_SIZE];
} Insert;

typedef struct {
	int       table_id;
	char      tuple[MAX_STAT_TUPLE_SIZE];
} Insert_stat;

typedef struct {
	int   table_id;
	int       att_id;
	char      att_val[MAX_ATT_PER_QP];
} Count;

typedef struct {
	int   table_id;
	int   att_id;
} Meta_Rdic;

typedef struct {
	int   table_id;
} Meta_Rdir;

typedef struct {
	int type;
	int srh_md;
	int size;
	int offset;
} Meta_Rdic_Ack;

typedef struct {
	int   quit_type;
} Quit;

#define MAX_MESG_SIZE (sizeof(Update) + 1)
#define MAX_STAT_MESG_SIZE (sizeof(Insert_stat) + 1)
/* Message structure */
typedef struct {
	long     mtype;
	int      EventName;
	int      SrcQid;
	int      R_type;
	int      dummy;
	// long     dummy;
	char      Mesg[MAX_MESG_SIZE];
} Message;

typedef struct {
	long     mtype;
	int      EventName;
	int      SrcQid;
	int      R_type;
	int      dummy;
	// long     dummy;
	char      Mesg[MAX_STAT_MESG_SIZE];
} Message_stat;

typedef struct {
	int E_Name;
	/*
	    union {
	        Select  sel;
	        Update  upd;
	        Delete  del;
	        Insert  ins;
	    } trans_req;
	*/
	char    Mesg[QUERY_UNIT_REQ_SIZE];
} Query_req;

typedef struct {
	long      mtype;
	int       EventName;
	int       SrcQid;
	int       R_type;
	//long      dummy;
	int      dummy;
	Query_req query[MULTI_COUNT];
} Message_Multi_Req;

/* Reply Data Structure */
struct _UDI_ack {
	int       tup_id;
};

typedef struct _UDI_ack     Update_Ack;
typedef struct _UDI_ack     Delete_Ack;
typedef struct _UDI_ack     Insert_Ack;

typedef struct {
	int       tup_id;
	char      tuple[MAX_TUPLE_SIZE];
} Select_Ack;

typedef struct {
	int     cursor_id;
	int     table_id;
	int     tup_id;
	char    tuple[MAX_TUPLE_SIZE];
} Cursor_Ack;

typedef struct {
	int tup_id;
	int count;
} Count_Ack;

typedef struct {
	int   error_code;
} Request_Fail;

typedef struct {
	int table_id;
	int numKey;
	int numAtt;
	int tupleSize;
	int tupleSpaceCnt;
	int storedTupleCnt;
	int firstFree;
	int needAlign;
} Meta_Rdir_Ack;


typedef struct {
	int E_Name;
	char    Mesg[QUERY_UNIT_ACK_SIZE];
} Query_ack;

typedef struct {
	long      mtype;
	int       EventName;
	int       SrcQid;
	int       R_type;
	//int       InstanceId;
	int      dummy;
	Query_ack ack[MULTI_COUNT];
} Message_Multi_Ack;


/* Size of each message */
#define S_S_ACK                 sizeof(Select_Ack)
#define S_U_ACK                 sizeof(Update_Ack)
#define S_D_ACK                 sizeof(Delete_Ack)
#define S_I_ACK                 sizeof(Insert_Ack)
#define S_O_ACK                 sizeof(Cursor_Ack)
#define S_C_ACK                 sizeof(Count_Ack)
#define S_R_FAIL                sizeof(Request_Fail)
#define S_RDIC_ACK              sizeof(Meta_Rdic_Ack)
#define S_RDIR_ACK              sizeof(Meta_Rdir_Ack)
#define S_MSG 					sizeof(Message)
typedef struct {
	int       instance_id ;
	int       tup_id ;
	int       count ;
	char      tuple [MAX_TUPLE_SIZE] ;
	union {
		Meta_Rdic_Ack           rdic ;
		Meta_Rdir_Ack           rdir ;
	} meta ;
} Result ;

#endif
