.KEEP_STATE:
include ${CONFIG_HOME}/Make.cf
CC = gcc

SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)

TARGET = libflkdia.a

.c.o:
	$(CC) -c -g  $(CFLAGS) $<

all: $(TARGET) $(TEST_APP)

$(TARGET): $(OBJS)
	$(AR) $@ $(OBJS)

install:
	$(CP) $(TARGET) $(DIA_LIB_DIR)
	$(CP) *.def $(DIA_INC_DIR)
	$(CP) *.h $(DIA_INC_DIR)

clean: 
	$(RM) $(TARGET) *.o core*

