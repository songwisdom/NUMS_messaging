include $(MAKE_HOME)/Make.cf

INCLUDE = -I/usr/include/libxml2 -I/usr/include/mysql -I$(INC_DIR)/gwlib -I$(INC_DIR)/config -I$(INC_DIR)/dbconn \
		  -I$(INC_DIR)


LIBRARY = -lpthread -lxml2 -lmysqlclient_r -lflkgw -lssl -lcfg -ldbconn

TARGET = testu

SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)

.c.o :
	$(CC) -g -Wall -c $(CFLAGS) $(INCLUDE) $<

all : $(TARGET)

$(TARGET) : $(OBJS) 
	$(CC) -g -o $(TARGET) $(OBJS) $(COMMON_LDFLAGS) $(LIBRARY)

install 	:
	$(CP) $(TARGET) $(BIN_DIR)

clean:
	$(RM) ${TARGET} *.o core.*
