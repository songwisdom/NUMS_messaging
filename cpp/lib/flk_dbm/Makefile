include $(SMSC_HOME)/sim/config/Make.cf
.KEEP_STATE:

LDFLAGS     = -L${SIM_LIB_PATH} 

OBJS        = dbmclose.o dbmdelete.o dbmdirfno.o dbmfetch.o dbminit.o dbmopen.o dbmpagfno.o dbmrdonly.o \
			dbmseq.o dbmstore.o gdbmclose.o gdbmdelete.o gdbmerrno.o gdbmexists.o gdbmfdesc.o \
			gdbmfetch.o gdbmopen.o gdbmreorg.o gdbmseq.o gdbmsetopt.o gdbmstore.o gdbmsync.o \
			update.o falloc.o bucket.o findkey.o global.o hash.o

CFLAGS      = ${CCOPTIONS} ${COMPANY_TYPE} $(DEBUG) $(TEST) $(INCDIR) $(INCDIR)/flk_dbm

TARGET      = libflkdbm.a 

.c.o: 
	$(CC) -c  $(CFLAGS) $<

all:	$(TARGET)

$(TARGET) :  $(OBJS) 
	$(AR) $(TARGET) $(OBJS) 

touch: 
	$(TOUCH) *.c 

clean: 
	${RM} -f ${TARGET} *.o core 

install: 
	${CP} ${TARGET} $(SIM_LIB_PATH) 

delobj : 
	${RM} ${TARGET}

