#ifndef __MDBDATASTRUCT_H_
#define __MDBDATASTRUCT_H_

/* seoki one line add */
#include "MdbCommon.h"

/* For Sld loader */
/* System Loading Data Structure */
typedef struct {
	int    	sld_adr;
	int     sld_ver;
	int     no_tup_rdir;
	int    	rdir_adr;
} DB_File_header;
//} __attribute__ ((packed)) DB_File_header;
typedef struct {
	int     rel_id;
	int     rdic_adr;
	int     numKey;
	int     numAtt;
	int     tupleSize;
	int    	rel_adr;
	int     tupleSpaceCnt;
	int     used_tup;
	int     firstFree;
} File_Rdir;
//} __attribute__ ((packed))  File_Rdir;

typedef struct {
	int     type;
	int     srh_md;
	int     size;
} File_Rdic;
//} __attribute__ ((packed))  File_Rdic;

////////////////////////////////////////

/* System Loading Data Structure */
/* FR220 For MMDB Safe */
typedef struct {
	char    	*sld_adr;
	long     sld_ver;
	long     no_tup_rdir;
	char    	*rdir_adr;
} DB_header;

typedef struct {
	long     table_id;
	long     numKey;
	long     numAtt;
	long     tupleSize;
	long     tupleSpaceCnt;
	long     storedTupleCnt;
	long     firstFree;
	long     needAlign;
} TableDir;

typedef struct {
	int     type;
	int     srh_md;
	int     size;
	int     offset;
} AttDir;

typedef struct {
	long     rel_id;
	char    *rdic_adr;
	long     numKey;
	long     numAtt;
	long     tupleSize;
	char    *rel_adr;
	long     tupleSpaceCnt;
	long     used_tup;
	long     firstFree;
} Rdir;

typedef struct {
	int     type;
	int     srh_md;
	int     size;
} Rdic;
/* On-line Backup Structure */
typedef struct {
	int     dirtyFlagCnt;
	char    dirtyFlag[1];
} DirtyFlagTable;

/* ML-Hash Structure */
typedef struct _Element {
	int             tupId;
	int 			reserved;
	char            key[MAX_HASH_KEY_SIZE];
	struct _Element *next;
} Element;

typedef struct {
	int NumKey;
	int TotSize;
	int TotFreeNumElement;
	Element freeElement;
} Index_header;

#define DirSize     256
#define SegSize     1024

typedef  struct _Index {
	int 	tableId;
	int 	attId;
	int     numElem;
	int     hashRange;
	int     numBucket;
	int     nextSplit;
	int     keyLength;
	int 	reserved;
	Element *dir[DirSize][SegSize];
} Index;

/* T-Tree Structure */
typedef struct _pair {
	int     tupId;
	char    *key;
} Pair;
typedef struct _Tnode {
	int tnodeId;
	int balanceFactor;
	int     leftOrRight;
	int     depthCnt;
	char    childCnt;
	int     freeSpace;
	int     keyCnt;
	int     keyLen;

	Pair    pair[MAX_KEY_IN_A_T_NODE];

	struct _Tnode  *leftChild;
	struct _Tnode  *rightChild;
	struct _Tnode  *parents;
	struct _Tnode  *next;
} Tnode;

typedef  struct _cursor {
	int     tableId;
	Tnode   *curTnode;
	int     curKeyCnt;
	int     tupId;
} CURSOR;

/* Memory Management Structure */
/*
struct list {
    struct list *next;
};
*/

struct elist {
	struct elist *next;
};

/* TODO linux porting
typedef union {
    struct {
        int type;
        union {
            struct {
                int nfree;
                int next;
                int prev;
                struct elist *first;
            } frag;
            int size;
        } info;
    } busy;

    struct {
        int size;
        int next;
        int prev;
    } free;
} malloc_info;
*/
/*---------------------------------------------*/
#define S_INT                 sizeof(int)
#define S_DBH                 sizeof(DB_header)
#define S_INDH                sizeof(Index_header)
#define S_RDIR                sizeof(Rdir)
#define S_RDIC                sizeof(Rdic)
#define S_ELM                 sizeof(Element)
#define S_INX                 sizeof(Index)

#define S_F_DBH 			  sizeof(DB_File_header)
#define S_F_RDIR 			  sizeof(File_Rdir)
#define S_F_RDIC 			  sizeof(File_Rdic)

#endif


