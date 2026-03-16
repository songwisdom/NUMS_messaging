include ${HOME}/config/Make.cf

SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)

#OBJS = accesslog.o dbpool.o dbpool_sqlite3.o gw_uuid.o http.o mime.o pki.o smpp_pdu_skt.o wap_addr.o wsp_pdu.o \
	   cfg.o dbpool_mysql.o dict.o gwlib.o ipcs.o protected.o smpp_sock.o wap_events.o wsp_strings.o \
	   dbpool_oracle.o fdset.o gwmem-check.o key_value.o msg.o pstat.o socket.o watch_dog.o wtls.o \
	   conn.o dbpool_pgsql.o flkini.o gwmem-native.o list.o octstr.o regex.o ssl.o wsp.o wtls_pdu.o \
	   counter.o dbpool_sdb.o gw-prioqueue.o gwpoll.o log.o parse.o semaphore.o thread.o wsp_caps.o \
	   date.o dbpool_sqlite.o gw-rwlock.o gwthread-pthread.o md5.o pcre.o smpp_pdu.o utils.o wsp_headers.o 

OBJS = conn.o  counter.o  date.o  dict.o  fdset.o  gw-rwlock.o  gwlib.o  gwmem-check.o  gwmem-native.o  gwpoll.o  gwthread-pthread.o  http.o  list.o  log.o  octstr.o  protected.o  regex.o  socket.o  thread.o  utils.o flkini.o key_value.o ipcs.o 

TARGET = libdiagw.a

.c.o:
	$(CC) $(CFLAGS) $<

all: $(TARGET)

$(TARGET): $(OBJS)
	$(AR) $@ $(OBJS)

install:
	$(CP) $(TARGET) /home/flksctp/lib/

clean: 
	$(RM) $(TARGET) *.o core*
