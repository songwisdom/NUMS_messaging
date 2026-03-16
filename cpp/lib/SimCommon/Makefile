include $(SMSC_HOME)/sim/config/Make.cf
.KEEP_STATE:

LDFLAGS     = -L${SIM_LIB_PATH} 

OBJS        = EProfile.o flk_string.o libipc.o sim_table.o util.o util_argument.o libdbg.o libnet.o lib_errmsg.o libtime.o

CFLAGS      = ${CCOPTIONS} ${COMPANY_TYPE} $(DEBUG) $(TEST) $(INCDIR) $(INCDIR)/SimCommon

TARGET      = libSimCommon.a

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

