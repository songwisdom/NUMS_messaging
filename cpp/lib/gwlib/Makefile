include $(SMSC_HOME)/sim/config/Make.cf

SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)

#OBJS = 	gw-prioqueue.o \
		xmlrpc.o \
		mms_cfg.o \
		dict.o \
		utils.o \
		http.o \
		regex.o \
		conn.o \
		accesslog.o \
		parse.o \
		mms_msg.o \
		octstr.o \
		pki.o \
		msg.o \
		list.o \
		mms_util.o \
		dbpool_sdb.o \
		socket.o \
		dbpool_pgsql.o \
		date.o \
		dbpool_mysql.o \
		mms_mm7soap.o \
		counter.o \
		ssl.o \
		pcre.o \
		fdset.o \
		mms_queue.o \
		mms_strings.o \
		mime.o \
		wap_addr.o \
		gwthread-pthread.o \
		mms_uaprof.o \
		wap_events.o \
		wsp.o \
		gwpoll.o \
		dbpool_sqlite3.o \
		dbpool_oracle.o \
		gwlib.o \
		md5.o \
		wsp_caps.o \
		gwmem-native.o \
		charset.o \
		thread.o \
		wtls.o \
		semaphore.o \
		log.o \
		wsp_headers.o \
		wsp_pdu.o \
		dbpool.o \
		gw-rwlock.o \
		dbpool_sqlite.o \
		wsp_strings.o \
		cfg.o \
		gwmem-check.o \
		protected.o \
		gw_uuid.o \
		wtls_pdu.o \
		smpp_pdu.o \
		smpp_sock.o \
		smpp_pdu_skt.o \
		key_value.o \
		flkini.o \
		pstat.o

TARGET = libflkgwsim.a

CFLAGS = -g -c -D_REENTRANT -D_FILE_OFFSET_BITS=64 -D_SMSC_SYSTEM_LINUX -I/usr/include $(INCDIR) $(GWINC)

.c.o:
	$(CC) $(CFLAGS) $<

all: $(TARGET)

$(TARGET): $(OBJS)
	$(AR) $@ $(OBJS)

install:
	$(CP) $(TARGET) ~/sim/lib/

clean: 
	$(RM) $(TARGET) *.o core*
