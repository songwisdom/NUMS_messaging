.KEEP_STATE:
include $(CONFIG_HOME)/Make.cf
CC = cc

SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)

TARGET = testu

.c.o:
	$(CC) -g $(CFLAGS) $<

all: $(TARGET) $(TEST_APP)

$(TARGET): $(OBJS)
	gcc -o $(TARGET) $(OBJS) $(LDFLAGS)

install:
	$(CP) $(TARGET) $(DBAGENT_LIB_DIR)

clean: 
	$(RM) $(TARGET) *.o core*
