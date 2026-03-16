include $(SMSC_HOME)/sim/config/Make.cf
.KEEP_STATE:

LDFLAGS     = -L${SIM_LIB_PATH} 

OBJS        = nnlog.o SimLog.o

CFLAGS      = ${CCOPTIONS} ${COMPANY_TYPE} $(DEBUG) $(TEST) $(INCDIR)

TARGET      = libDebug.a 

.c.o: 
	$(CC) -c  $(CFLAGS) $<

all:	$(TARGET)

$(TARGET) :  $(OBJS) 
	$(AR) $@ $(OBJS) 

touch: 
	$(TOUCH) *.c 

clean: 
	${RM} -f ${TARGET} *.o core 

install: 
	${CP} ${TARGET} $(SIM_LIB_PATH) 

delobj : 
	${RM} ${TARGET}

