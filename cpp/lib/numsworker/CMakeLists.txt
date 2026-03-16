file(GLOB SRC_FILES CONFIGURE_DEPENDS
	numsworker.cpp TcpClient.cpp
)
set_source_files_properties( ${SRC_FILES}  PROPERTIES LANGUAGE CXX )

add_definitions( "-DTCAP_WHITE -D__GSM_ROUTE_PC__" )

include_directories(${INCLUDES})

add_library(numsworker STATIC ${SRC_FILES})

install(TARGETS numsworker DESTINATION ${LIB_DIR})

