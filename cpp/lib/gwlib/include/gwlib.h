/* 
 * File:   flk_lib.h
 * Author: hajima
 *
 * Created on 2015년 4월 16일 (목), 오후 4:57
 */

#ifndef FLK_GW_LIB_H
#define	FLK_GW_LIB_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
	/////////
#include "gw-config.h"

//#include "gw-getopt.h"
#include "gwpoll.h"

#include "utils.h"
#include "log.h"
#include "thread.h"
#include "gwthread.h"
#include "gwmem.h"
#include "socket.h"
//#include "cfg.h"
#include "date.h"
#include "http.h"
#include "octstr.h"
#include "list.h"
#include "fdset.h"
#include "gwassert.h"
#include "counter.h"
//#include "charset.h"
#include "conn.h"
//#include "ssl.h"
//#include "parse.h"
#include "protected.h"
//#include "accesslog.h"
#include "dict.h"
//#include "gwlib_sem.h"
//#include "xmlrpc.h"
//#include "md5.h"
//#include "gw_uuid.h"
#include "gw-rwlock.h"
//#include "gw-prioqueue.h"
//#include "keylist.h"
//#include "flkini.h"
//#include "flkpstat.h"
//#include "flkipc.h"

void gwlib_assert_init(void);
void gwlib_init(void);
void gwlib_shutdown(void);

#ifdef NO_GWASSERT
#define gwlib_assert_init() ((void) 0)
#endif


#ifdef	__cplusplus
}
#endif

#endif	/* FLK_LIB_H */

