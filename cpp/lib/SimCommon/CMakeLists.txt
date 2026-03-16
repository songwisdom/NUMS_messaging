file(GLOB SRC_FILES CONFIGURE_DEPENDS
	EProfile.c flk_string.c libipc.c sim_table.c util.c util_argument.c libdbg.c libnet.c lib_errmsg.c libtime.c
)
set_source_files_properties( ${SRC_FILES}  PROPERTIES LANGUAGE CXX )

add_definitions( "-DTCAP_WHITE -D__GSM_ROUTE_PC__" )

include_directories(${INCLUDES})

add_library(SimCommon STATIC ${SRC_FILES})

install(TARGETS SimCommon DESTINATION ${LIB_DIR})



