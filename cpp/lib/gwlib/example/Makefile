CC = gcc

CCOPTIONS = -Wall -D_REENTRANT=1 -I. -g -O2 -D_XOPEN_SOURCE=600 -D_BSD_SOURCE -D_LARGE_FILES= 

#LIBXML_INC = -I/usr/include/libxml2

CFLAGS = $(CCOPTIONS) -I/usr/include $(LIBXML_INC) -I. -D_DBG_MODE_ -I../

SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)

TARGET = testu

.c.o:
	$(CC) -c $(CFLAGS) $(INCLUDE) $<

all : $(TARGET)

$(TARGET) : $(OBJS)
	gcc -o $@ $(OBJS) -L../ -lPushClient -lpthread
clean :
	rm -f $(TARGET) *.o core

