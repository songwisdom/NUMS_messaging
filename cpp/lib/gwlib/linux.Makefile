include ${HOME}/config/Make.cf

SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)


OBJS = conn.o  counter.o  date.o  dict.o  fdset.o  gw-rwlock.o  gwlib.o  gwmem-check.o  gwmem-native.o  gwpoll.o  gwthread-pthread.o  http.o  list.o  log.o  octstr.o  protected.o  regex.o  socket.o  thread.o  utils.o flkini.o key_value.o ipcs.o

TARGET = libdiagw.a

.c.o:
	$(CC) $(CFLAGS) $<

all: $(TARGET)

$(TARGET): $(OBJS)
	$(AR) $@ $(OBJS)

install:
	$(CP) $(TARGET) ~/lib/

clean: 
	$(RM) $(TARGET) *.o core*
