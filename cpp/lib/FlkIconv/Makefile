include	$(SMSC_HOME)/sim/config//Make.cf

OBJS   = contentsConvert.o flk_iconv.o

CFLAGS = -g -c -D_REENTRANT -D_FILE_OFFSET_BITS=64 -D_SMSC_SYSTEM_LINUX -I/usr/include $(INCDIR) $(GWINC)

TARGET = libsim_iconv.a

.c.o :
	$(CC) $(CFLAGS) $<

all: $(TARGET)

$(TARGET): $(OBJS)
	$(AR) $@ $(OBJS)

install:
	$(CP) -f $(TARGET) ~/sim/lib/

clean:
	$(RM) $(TARGET) *.o core*
