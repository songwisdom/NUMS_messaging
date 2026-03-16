.KEEP_STATE:
include ${CONFIG_HOME}/Make.cf
CC = cc

SRCS = $(wildcard *.c)
#OBJS = $(SRCS:.c=.o)
OBJS = avp.o avp_group.o avp_group_list.o avp_list.o debug.o decoder.o dia_intf.o diameter.o encoder.o util.o

TARGET = libflkdia.a

.c.o:
	$(CC) -c -g  $(CFLAGS) $<

all: $(TARGET) $(TEST_APP)

$(TARGET): $(OBJS)
	$(AR) $@ $(OBJS)

install:
	$(CP) $(TARGET) $(DGW_LIB_DIR)
	$(CP) *.def $(DGW_INC_DIR)
	$(CP) *.h $(DGW_INC_DIR)

clean: 
	$(RM) $(TARGET) *.o core*

