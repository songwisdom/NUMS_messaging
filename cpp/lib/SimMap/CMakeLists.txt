file(GLOB SRC_FILES CONFIGURE_DEPENDS
	V3_ObjectId.c MapUtil.c MapV3Address.c encV3_Extension.c encV3_MO_ForwardSM_Arg.c 
	enc2G_637.c enc2G_SMDPP.c enc2G_Addr.c TcapHandler.c MapAddrUtil.c 
	encV3_RoutingInfoForSM_Res.c decV3_RoutingInfoForSM_Arg.c decV3_Extension.c
	decSMDPP.c decV3_MT_ForwardSM_Arg.c GsmTpduStruct.c decGsmStruct.c 
	decV3_AnyTimeSubsInterrogation_Arg.c decV3_ReportSM_DeliveryStatus_Arg.c 
	decV3_MO_ForwardSM_Arg.c
)
set_source_files_properties( ${SRC_FILES}  PROPERTIES LANGUAGE CXX )

add_definitions( "-DTCAP_WHITE -D__GSM_ROUTE_PC__" )

include_directories(${INCLUDES})

add_library(SimMapCom STATIC ${SRC_FILES})

install(TARGETS SimMapCom DESTINATION ${LIB_DIR})



