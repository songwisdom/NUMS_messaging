.KEEP_STATE:
include $(HOME)/etc/Make.cf
CC = gcc
#CCOPTIONS = -Wall -D_REENTRANT=1 -I. -g -O2 -D_XOPEN_SOURCE=600 -D_BSD_SOURCE -D_LARGE_FILES=
#LIBXML_INC = -I/usr/include/libxml2
#CFLAGS = $(CCOPTIONS) -I/usr/include -I/usr/include/mysql -I$(DBAGENT_INC_DIR)/gwlib $(LIBXML_INC) -I. -D_DBG_MODE_

SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)

TARGET = testu

.c.o:
	$(CC) -c -g $(CFLAGS) $(INCLUDE) -I../ $<

all: $(TARGET) $(TEST_APP)

$(TARGET): $(OBJS)
	gcc -o $(TARGET) $(OBJS) $(LDFLAGS) -L../ -lflkdia

install:
	$(CP) $(TARGET) $(DBAGENT_LIB_DIR)

clean: 
	$(RM) $(TARGET) *.o core*

