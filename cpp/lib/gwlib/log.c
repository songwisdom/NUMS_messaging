/* ==================================================================== 
 * The Kannel Software License, Version 1.0 
 * 
 * Copyright (c) 2001-2009 Kannel Group  
 * Copyright (c) 1998-2001 WapIT Ltd.   
 * All rights reserved. 
 * 
 * Redistribution and use in source and binary forms, with or without 
 * modification, are permitted provided that the following conditions 
 * are met: 
 * 
 * 1. Redistributions of source code must retain the above copyright 
 *    notice, this list of conditions and the following disclaimer. 
 * 
 * 2. Redistributions in binary form must reproduce the above copyright 
 *    notice, this list of conditions and the following disclaimer in 
 *    the documentation and/or other materials provided with the 
 *    distribution. 
 * 
 * 3. The end-user documentation included with the redistribution, 
 *    if any, must include the following acknowledgment: 
 *       "This product includes software developed by the 
 *        Kannel Group (http://www.kannel.org/)." 
 *    Alternately, this acknowledgment may appear in the software itself, 
 *    if and wherever such third-party acknowledgments normally appear. 
 * 
 * 4. The names "Kannel" and "Kannel Group" must not be used to 
 *    endorse or promote products derived from this software without 
 *    prior written permission. For written permission, please  
 *    contact org@kannel.org. 
 * 
 * 5. Products derived from this software may not be called "Kannel", 
 *    nor may "Kannel" appear in their name, without prior written 
 *    permission of the Kannel Group. 
 * 
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESSED OR IMPLIED 
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES 
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
 * DISCLAIMED.  IN NO EVENT SHALL THE KANNEL GROUP OR ITS CONTRIBUTORS 
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY,  
 * OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT  
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR  
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,  
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE  
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,  
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 
 * ==================================================================== 
 * 
 * This software consists of voluntary contributions made by many 
 * individuals on behalf of the Kannel Group.  For more information on  
 * the Kannel Group, please see <http://www.kannel.org/>. 
 * 
 * Portions of this software are based upon software originally written at  
 * WapIT Ltd., Helsinki, Finland for the Kannel project.  
 */ 

/*
 * log.c - implement logging functions
 */

#include "gwlib.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>
#include <stdarg.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#ifdef HAVE_EXECINFO_H
#include <execinfo.h>
#endif

#if HAVE_SYSLOG_H
#include <syslog.h>
#else

/*
 * If we don't have syslog.h, then we'll use the following dummy definitions
 * to avoid writing #if HAVE_SYSLOG_H everywhere.
 */

enum {
	LOG_PID, LOG_DAEMON, LOG_DEBUG, LOG_INFO, LOG_WARNING, LOG_ERR, LOG_ALERT
};

static void openlog(const char *ident, int option, int facility)
{
}

static void syslog(int translog, const char *buf)
{
}

static void closelog(void)
{
}
#endif


static char g_flow_log_file[256];
/*
 * List of currently open log files.
 */
#define MAX_LOGFILES 128
#define LOG_FILE_SIZE 128
static char g_log_proc[512];
typedef struct {
	FILE *file;
	int minimum_output_level;
	char filename[LOG_FILE_SIZE + 1]; /* to allow re-open */
	char name[LOG_FILE_SIZE + 1]; /* to allow re-open */
	enum excl_state exclusive;
	char ext[16];
} stlogfiles;
static stlogfiles logfiles[MAX_LOGFILES];
static int num_logfiles = 0;

static bool flow_log_level;


/*
 * Mapping array between thread id and logfiles[] index.
 * This is used for smsc specific logging.
 */
#define THREADTABLE_SIZE 1024
static unsigned int thread_to[(long)THREADTABLE_SIZE];

static Octstr *bind_cmt[THREADTABLE_SIZE];


/*
 * Ensure we use the real threadtable slot number to map the thread id
 * instead of the thread id reported by gwthread_self()
 */
#define thread_slot() \
	(gwthread_self() % THREADTABLE_SIZE)


/*
 * List of places that should be logged at debug-level.
 */
#define MAX_LOGGABLE_PLACES (10*1000)
static char *loggable_places[MAX_LOGGABLE_PLACES];
static int num_places = 0;


/*
 * Reopen/rotate locking things.
 */
//static RWLock rwlock;
static Mutex *rwlock;

/*
 * Syslog support.
 */
static int sysloglevel;


/*
 * Make sure stderr is included in the list.
 */
static void add_stderr(void)
{
	if (logfiles[0].file == stderr)
		return;

	logfiles[0].file = stderr;
	logfiles[0].minimum_output_level = GW_DEBUG;
	logfiles[0].exclusive = GW_EXCL;
    ++num_logfiles;
}

void log_init(void)
{
	unsigned long i;

	/* Initialize rwlock */
	//gw_rwlock_init_static(&rwlock);

	rwlock = mutex_create();

    /* default all possible thread to logging index 0, stderr */
    for (i = 0; i <= THREADTABLE_SIZE; i++) {
        thread_to[i] = 0;
    }

	add_stderr();

}

void log_shutdown(void)
{
	log_close_all();
	/* destroy rwlock */
	//gw_rwlock_destroy(&rwlock);
	mutex_destroy(rwlock);
}

void log_set_output_level(enum output_level level)
{
	if (logfiles[0].file == stderr) {
		logfiles[0].minimum_output_level = level;
	}
}

void log_set_log_level(enum output_level level)
{
	/* change everything but stderr */
	if (logfiles[1].file != stderr) {
		logfiles[1].minimum_output_level = level;
		info(0, "Changed logfile `%s' to level `%d'.", logfiles[1].filename, level);
	}
}


void log_close_all(void)
{
	fclose(stderr);
}


int log_open(char *filename, int level, char *proc_name )
{
	int  i;
	struct tm tm;
	time_t t;

	//gw_rwlock_wrlock(&rwlock);
	mutex_lock(rwlock);

	if (strlen(filename) > LOG_FILE_SIZE) {
		//gw_rwlock_unlock(&rwlock);
		mutex_unlock(rwlock);
		error(0, "Log filename too long: `%s'.", filename);
		return -1;
	}

	time(&t);
	tm = gw_localtime(t);

	logfiles[1].file = NULL ;
	logfiles[1].minimum_output_level = level;
	logfiles[1].exclusive = GW_EXCL;
	sprintf(logfiles[1].ext, "%04d%02d%02d", tm.tm_year + 1900, tm.tm_mon+1, tm.tm_mday);

	strcpy(logfiles[1].filename, filename);
	num_logfiles = 2;
	i = num_logfiles - 1;
	if ( proc_name == NULL || strlen(proc_name) == 0 )
		sprintf(g_log_proc, "%d", getpid());
	else
		strcpy(g_log_proc, proc_name) ;

	//sprintf(g_flow_log_file, "%s_flow", logfiles[i].filename);

	mutex_unlock(rwlock);


	info(0, "Added logfile `%s' with level `%d'.", filename, level);

	return i;
}

int log_thread_open(char *filename, int level)
{
    FILE *f = NULL;
    int i;
	struct tm tm;
	time_t t;
    
    //gw_rwlock_wrlock(&rwlock);
    mutex_lock(rwlock);

    if (num_logfiles == MAX_LOGFILES) {
        //gw_rwlock_unlock(&rwlock);
        mutex_unlock(rwlock);
        error(0, "Too many log files already open, not adding `%s'",
              filename);
        return -1;
    }

    if (strlen(filename) > FILENAME_MAX) {
        //gw_rwlock_unlock(&rwlock);
        mutex_unlock(rwlock);
        error(0, "Log filename too long: `%s'.", filename);
        return -1;
    }

    /*
     * Check if the file is already opened for logging.
     * If there is an open file, then assign the file descriptor
     * that is already existing for this log file.
     */
    for (i = 0; i < num_logfiles && f == NULL; ++i) {
        if (strcmp(logfiles[i].filename, filename) == 0)
            f = logfiles[i].file;
    }

    /* if not previously opened, then open it now */
	/*
    if (f != NULL) {
		fclose(f);
    }
	*/
    
	time(&t);
	tm = gw_localtime(t);

    logfiles[num_logfiles].file = NULL;
    logfiles[num_logfiles].minimum_output_level = level;
    logfiles[num_logfiles].exclusive = GW_EXCL;
    strcpy(logfiles[num_logfiles].filename, filename);
	sprintf(logfiles[num_logfiles].ext, "%04d%02d%02d", tm.tm_year + 1900, tm.tm_mon+1, tm.tm_mday);
    ++num_logfiles;
    i = num_logfiles - 1;
    //gw_rwlock_unlock(&rwlock);
	mutex_unlock(rwlock);

	log_thread_to(i);

    info(0, "Added logfile `%s' with level `%d'. index[%i]", filename, level, i);

    return i;
}


#define FORMAT_SIZE (4096)
static void format(char *buf, int level, const char *place, int e,
		const char *fmt, char *ext)
{
	static char *tab[] = {
		"DEBUG",
		"INFO ",
		"WARN ",
		"ERROR",
		"PANIC",
		"LOG  "
	};

	struct tm tm;
	char *p, prefix[1024];
	long pos;

	struct timeval tv;

	p = prefix;


	gettimeofday(&tv, NULL);

	tm = gw_localtime(tv.tv_sec);

	if ( level < 0 || level > 5 )
		level = 5;

	pos = sprintf(p, "[%02d:%02d:%02d.%06ld] [%s(%d/%ld)] [%s] ",
			tm.tm_hour, tm.tm_min, tm.tm_sec, tv.tv_usec, g_log_proc, getpid(), gwthread_self(),tab[level]);

	sprintf(ext, "%04d%02d%02d-%02d", tm.tm_year + 1900, tm.tm_mon+1, tm.tm_mday, tm.tm_hour);

	p += pos;
	if (place != NULL && *place != '\0')
		sprintf(p, "%s(%d): ", place, e); // debug mode e = line number

	if (level == GW_DEBUG || e == 0) {
		sprintf(buf, "%s%s\n", prefix, fmt);
	} else if ( level != GW_DEBUG ) {
		sprintf(buf, "%s%s\n%sSystem error %d: %s\n",
				prefix, fmt, prefix, e, strerror(e));
	}
}


static bool open_logfile(char *ext, stlogfiles *logfile)
{

//	struct stat buf;

	if ( logfile->exclusive != GW_EXCL )
		return false;
	
	if ( logfile->file == stderr )
		return true;

	sprintf(logfile->name, "%s.%s", logfile->filename, ext);

	logfile->file = fopen(logfile->name, "a");
	if ( logfile->file == NULL ) {
		perror("fopen failed");
		return false;
	}

	return true;
}

/*
 * Almost all of the message printing functions are identical, except for
 * the output level they use. This macro contains the identical parts of
 * the functions so that the code needs to exist only once. It's a bit
 * more awkward to edit, but that can't be helped. The "do {} while (0)"
 * construct is a gimmick to be more like a function call in all syntactic
 * situation.
 */

#define FUNCTION_GUTS(level, place) \
	do { \
		int i; \
		char buf[FORMAT_SIZE]; \
		va_list args; \
		char ext[16]; \
		\
		format(buf, level, place, err, fmt, ext); \
        mutex_lock(rwlock); \
		for (i = 0; i < 2; ++i) { \
			if ( level >= logfiles[i].minimum_output_level ) { \
				if ( open_logfile(ext, &logfiles[i]) == true ) { \
					va_start(args, fmt); \
					vfprintf(logfiles[i].file, buf, args); \
					va_end(args); \
					if ( logfiles[i].file != stderr ) { \
						fclose(logfiles[i].file); \
					} \
				} \
			} \
		} \
        mutex_unlock(rwlock); \
	} while (0)

#define FUNCTION_GUTS_THREAD(level, place) \
	do { \
		char buf[FORMAT_SIZE]; \
		va_list args; \
		char ext[16]; \
		\
		format(buf, level, place, err, fmt, ext); \
        mutex_lock(rwlock); \
		if ( level >= logfiles[e].minimum_output_level ) { \
			if ( open_logfile(ext, &logfiles[e]) == true ) { \
				va_start(args, fmt); \
				vfprintf(logfiles[e].file, buf, args); \
				va_end(args); \
				fclose(logfiles[e].file); \
			} \
		} \
        mutex_unlock(rwlock); \
	} while (0)



#ifdef HAVE_BACKTRACE
static void PRINTFLIKE(2,3) gw_panic_output(int err, const char *fmt, ...)
{
	FUNCTION_GUTS(GW_PANIC, "");
}
#endif


void gw_panic(int err, const char *fmt, ...)
{
	/*
	 * we don't want PANICs to spread accross smsc logs, so
	 * this will be always within the main core log.
	 */
	FUNCTION_GUTS(GW_PANIC, "");

#ifdef HAVE_BACKTRACE
	{
		void *stack_frames[50];
		size_t size, i;
		char **strings;

		size = backtrace(stack_frames, sizeof(stack_frames) / sizeof(void*));
		strings = backtrace_symbols(stack_frames, size);

		if (strings) {
			for (i = 0; i < size; i++)
				gw_panic_output(0, "%s", strings[i]);
		}
		else { /* hmm, no memory available */
			for (i = 0; i < size; i++)
				gw_panic_output(0, "%p", stack_frames[i]);
		}

		/*
		 * Note: we don't free 'strings' array because gw_free could panic's and we
		 *       have endless loop with SEGFAULT at the end. And this doesn't care
		 *       us in any case, because we are panic's and exiting immediately. (alex)
		 */
	}
#endif

	/*
#ifdef SEGFAULT_PANIC
	*((char*)0) = 0;
#endif
	*/

	//exit(EXIT_FAILURE);
}

void error(int err, const char *fmt, ...) 
{
	int e;

	e = thread_to[thread_slot()];
	if ( e == 0 ) {
		FUNCTION_GUTS(GW_ERROR, "");
	} else {
		FUNCTION_GUTS_THREAD(GW_ERROR, "");
	}
}

void warning(int err, const char *fmt, ...) 
{
	int e;

	e = thread_to[thread_slot()];
	if ( e == 0 ) {
		FUNCTION_GUTS(GW_WARNING, "");
	} else {
		FUNCTION_GUTS_THREAD(GW_WARNING, "");
	}
}

void info(int err, const char *fmt, ...) 
{
	int e;

	e = thread_to[thread_slot()];
	if ( e == 0 ) {
		FUNCTION_GUTS(GW_INFO, "");
	} else {
		FUNCTION_GUTS_THREAD(GW_INFO, "");
	}
}

void debug(const char *place, int err, const char *fmt, ...) 
{
	int e;

	e = thread_to[thread_slot()];
	if ( e == 0 ) {
		FUNCTION_GUTS(GW_DEBUG, place);
	} else {
		FUNCTION_GUTS_THREAD(GW_DEBUG, place);
	}
}


void flow_log_open(const char *log_file)
{
	sprintf(g_flow_log_file, "%s_flow", log_file);
}

void flow_log(const char *fmt, ...)
{
	FILE *fp;
	char log_file[256];
	char buf[FORMAT_SIZE];
	va_list args;
	char ext[16];
	struct tm tm;
	struct timeval tv;
	char *p, prefix[1024];
	long pos;

	if ( flow_log_level == false )
		return;
	mutex_lock(rwlock); 

	gettimeofday(&tv, NULL);
	tm = gw_localtime(tv.tv_sec);

	sprintf(prefix, "[%02d:%02d:%02d.%06ld] ",
			tm.tm_hour, tm.tm_min, tm.tm_sec, tv.tv_usec);

	sprintf(ext, "%04d%02d%02d-%02d", tm.tm_year + 1900, tm.tm_mon+1, tm.tm_mday, tm.tm_hour);

	sprintf(log_file, "%s.%s", g_flow_log_file, ext);

	fp = fopen(log_file, "a");
	if ( fp != NULL ) {

		fputs(prefix, fp);
		va_start(args, fmt);
		vfprintf(fp, fmt, args);
		va_end(args);
		fputs("\n", fp);
		fclose(fp);
	}


	mutex_unlock(rwlock); 
}


void log_set_debug_places(const char *places) 
{
	char *p;

	p = strtok(gw_strdup(places), " ,");
	num_places = 0;
	while (p != NULL && num_places < MAX_LOGGABLE_PLACES) {
		loggable_places[num_places++] = p;
		p = strtok(NULL, " ,");
	}
}

void log_unbind_comment()
{
	long i = thread_slot();
	if ( bind_cmt[i] ) {
		octstr_destroy(bind_cmt[i]);
		bind_cmt[i] = NULL;
	}
}

void log_bind_comment(u_int msgid, int mtype, char *dest, int dlen, char *src, int slen)
{
	char *d = dest, *s = src;
	long th_id = thread_slot();
	
	if ( (d == NULL) || (dlen == 0) ) {
		d = "null";
		dlen = 4;
	}

	if ( (s == NULL) || (slen == 0) ) {
		s = "null";
		slen = 4;
	}

	if ( bind_cmt[th_id] )
		octstr_destroy(bind_cmt[th_id]);

	switch(mtype) {
		case MIN_TYPE_STRING :
			bind_cmt[th_id] = octstr_format("MsgID[%u : %08X] DEST[%s] SRC[%s] ", 
					msgid, msgid, (char *)d, (char *)s);
			break;
		case MIN_TYPE_BCD :
			{
		        /**          
        		 * 2012.01.04 Updated by mju
				 */
				Octstr *od = NULL;
				Octstr *os = NULL;
				if ( dlen > 0 ) {
					od = octstr_get_bcd_to_str((u_char *)d, dlen, 0);
				}
				if ( slen > 0 ) {
					os = octstr_get_bcd_to_str((u_char *)s, slen, 0);
				}
				bind_cmt[th_id] = octstr_format("MsgID[%u : %08X] DEST[%s] SRC[%s] ", 
						msgid, msgid, octstr_get_cstr(od), octstr_get_cstr(os));
				octstr_destroy(od);
				octstr_destroy(os);
			}
			break;
	}

	return;
}

void log_thread_to(unsigned int idx)
{
    long thread_id = thread_slot();

    if (idx > 0) 
        info(0, "Logging thread `%ld' to logfile `%s' with level `%d'.", 
             thread_id, logfiles[idx].filename, logfiles[idx].minimum_output_level);
    thread_to[thread_id] = idx;
}

void set_flow_log_onoff(bool flag)
{
	flow_log_level = flag;
}
