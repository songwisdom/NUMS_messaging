#ifndef __EMINMASK_H__
#define __EMINMASK_H__

#include <MdbDmaqi.h>
#include <MdbTableInfo.h>
#include "EIpc.h"
#ifndef MIN_SIZE
#define MIN_SIZE 12
#endif

#define MAX_ERROR_HISTORY 5

#define ACTIVE_HEAD_KEYIDX(idx)		 (idx > 1)? idx - 1 : 0
#define ACTIVE_HEAD_SEMIDX(key)		 ((key >> 4) % 10)


typedef struct {
	char 			  cpMin[MIN_SIZE];
	u_int 		  unTime;
	u_int 		  unMsgId;
	u_int		  unStartMsgId;
	u_int		  unManagementMsgId;
	u_int		  unExpressMsgId;
	u_int		  unNormalMsgId;
	u_int		  unReservationMsgId;
	u_int		  unFinishStartMsgId;
	u_int		  unFinishEndMsgId;
	int           nMsgCnt;
	int           nAttempts;
	int           nStartTime;
	u_int 	      unNextRetry;
	int           nFrame;
	int           nPlan;
	int           nStep;
	int           nRepeat;
	int           nStatus;
	int           nPeerStatus;
	int           nManagementStatus;
	int           nFailReason;
	int           npErrorHistory[MAX_ERROR_HISTORY];
} MinMaskDBType1;

/* 사용하는 구조체가 없어 일단 주석 처리함 by LCH 09.04.03
typedef struct
{
	int	            nMsgs;
	time_t	        lnNextRetry;
	int	            nAckTimeout;
	int		        nAttempts;
	int			    npErrorHistory[MAX_ERROR_HISTORY];
	int			    nError;
	time_t			lnStartedAt;
	int				nFrame;
	int				nPlan;
	int				nStep;
	int				nLocalError;
	time_t			lnLocalLastRetry;
	time_t			lnLocalStartedAt;
	int 			nNetworkError;
	time_t          lnNetworkLastRetry;
	time_t			lnNetworkStartedAt;
	char			cpSourMin[MIN_SIZE];
	int			    nTon;
	int				nNpi;
	u_long	ulMsgId;
} MinInfoType;
*/

// TransDBMS
typedef struct {
	char 			cpMin[MIN_SIZE];
	u_int 		unStartMsgId;
	u_int 		unNormalMsgId;
	int 		nMsgCnt;
} MinMaskDBType2;


typedef struct {
	int  nFlag;
	long lnMainNextPtr;
	long lnSubNextPtr;
} EHashRecordHeader;

typedef struct {
	int nShmKey;
	int nSemKey;
	int nKeySize;
	int nDataSize;
	int nDummySize;
	int nMaxSize;
	int nRecordSize;
	long lnToTalCnt;
} EHashMemDBHeaderType;

typedef struct {
	EHashMemDBHeaderType *spHeader;
	int nSemId;
	EShmType *spShm;
} EHashMemDBType;


#ifdef __cplusplus
//extern "C" {
#endif

int DeleteActiveHead(int nDbmsHandle, char *cpMDN);

int GetActiveHeadPtr(int nDbmsHandle, char *cpMDN, MinMaskDBType1 **spMask);


int GetReservationHeadTuple(
    int 				nDbmsHandle,
    stRDReserveHeadType	**spRsvdHead,
    time_t 				tTime);

int GetRetryHeadTuple(
    int 				nDbmsHandle,
    stRDRetryHeadType 	**spRetryHead,
    time_t 				tTime);

int GetReservationDataTuple(
    int 				nDbmsHandle,
    stRDReserveHeadType	*spRsvdHead,
    stRDReserveType		**spRsvdData);

int GetRetryDataTuple(
    int 				nDbmsHandle,
    stRDRetryHeadType 	*spRetryHead,
    stRDRetryType 		**spRetryData);

int DeleteReservationDataTuple(
    int 				nDbmsHandle,
    stRDReserveHeadType	*spRsvdHead,
    stRDReserveType		*spRsvdData);


int DeleteRetryDataTuple(
    int 				nDbmsHandle,
    stRDRetryHeadType 	*spRetryHead,
    stRDRetryType 		*spRetryData);

int DeleteReservationHeadTuple(
    int 				nDbmsHandle,
    stRDReserveHeadType	*spRsvdHead);

int DeleteRetryHeadTuple(
    int 				nDbmsHandle,
    stRDRetryHeadType 	*spRetryHead);

int InitReservationHeadTuple(
    int 				nDbmsHandle);


int InitRetryHeadTuple(
    int 				nDbmsHandle);


int InsertReservationHeadTuple(
    int 				nDbmsHandle,
    stRDReserveHeadType	*spRsvdHead);


int InsertRetryHeadTuple(
    int 				nDbmsHandle,
    stRDRetryHeadType 	*spRetryHead);


int InsertReservationDataTuple(
    int 				nDbmsHandle,
    stRDReserveHeadType	*spRsvdHead,
    stRDRetryType 		*spRetryData);

int InsertRetryDataTuple(
    int 				nDbmsHandle,
    stRDRetryHeadType 	*spRetryHead,
    stRDRetryType 		*spRetryData);

int InsertReservationData2DB(
    int 				nDbmsHandle,
    stRDReserveType 	*spRetryData);

int InsertRetryData2DB(
    int 				nDbmsHandle,
    stRDRetryType		*spRetryData);
int GetActiveHeadPtrTrans(int nDbmsHandle, char *cpMDN, MinMaskDBType2 **spMask);
int GetRcsHeadPtr(char *cpMDN, MinMaskDBType1 **spMask);
#ifdef __cplusplus
//}
#endif

#endif
