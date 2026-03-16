include	$(SMSC_HOME)/sim/config/Make.cf

.KEEP_STATE:

TEST		= -D_LONG_MSG_

#GSM			= -DGSM_VERSION
#DEBUG		= -D_DEBUG_ D_DLEVEL=1 -g 
#NOTI_R		= -DNOTI_ROUTE
G_ROUTE_PC	= -D__GSM_ROUTE_PC__


LIBRARY     = $(LIBDIR)
INCLUDE     = $(OCINCDIR) $(INCDIR) $(INCDIR)/SimMap \
				$(INCDIR)/SimCommon -I$(SMSC_HOME)/src/include

CC = cc
LDFLAGS     = $(OCLIBDIR) 
#	Taean for Dualactive -DDUALHANDLER

CFLAGS      = ${MAP_CCOPTIONS} -DTCAP_WHITE ${COMPANY_TYPE} $(DEBUG) $(G_ROUTE_PC) 

LDFLAGS_DUAL     = ${LIBRARY} -ltcpsock

OBJS        = V3_ObjectId.o MapUtil.o MapV3Address.o encV3_Extension.o encV3_MO_ForwardSM_Arg.o \
				enc2G_637.o enc2G_SMDPP.o enc2G_Addr.o TcapHandler.o MapAddrUtil.o \
				encV3_RoutingInfoForSM_Res.o decV3_RoutingInfoForSM_Arg.o decV3_Extension.o libDebugUtil.o \
				decSMDPP.o decV3_MT_ForwardSM_Arg.o GsmTpduStruct.o decGsmStruct.o \
				decV3_AnyTimeSubsInterrogation_Arg.o decV3_ReportSM_DeliveryStatus_Arg.o \
				decV3_MO_ForwardSM_Arg.o

TARGET     	= libSimMapCom.a 
.c.o:
	$(CC) -c  $(CFLAGS) $(INCLUDE) $<

all: 	$(TARGET) 

$(TARGET): 	$(OBJS) 
	$(AR) $@ $(OBJS)
#${CP} $(TARGET) $(SIM_LIB_PATH)
#rm -f $(TARGET) $(OBJS)
touch:
	$(TOUCH) *.c

clean:
	${RM} -f $(TARGET) *.o core

install:
	${CP} $(TARGET) $(SIM_LIB_PATH)
