.KEEP_STATE:
include ${CONFIG_HOME}/Make.cf
CC = gcc

SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)

TARGET = libflkdia.a

.c.o:
	$(CC) -c -g  -I/usr/local/include -I/usr/include/mysql -I/usr/include/libxml2 -I. -I/home/smsc/sim/src/include/dgw -I/home/smsc/sim/src/include/dgw/gwlib -I -I/home/smsc/sim/src/include/dgw/xhash -I/home/smsc/sim/src/lib/diameter -I/home/smsc/sim/src/include/gwlib $(CFLAGS) $<

all: $(TARGET) $(TEST_APP)

$(TARGET): $(OBJS)
	$(AR) $@ $(OBJS)

install:
	$(CP) $(TARGET) $(DIA_LIB_DIR)
	$(CP) *.def $(DIA_INC_DIR)
	$(CP) *.h $(DIA_INC_DIR)

clean: 
	$(RM) $(TARGET) *.o core*

