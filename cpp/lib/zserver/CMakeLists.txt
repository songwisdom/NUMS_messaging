file(GLOB SRC_FILES CONFIGURE_DEPENDS
	ThreadSafeQueue.cpp zmq_server.cpp 
)
set_source_files_properties( ${SRC_FILES}  PROPERTIES LANGUAGE CXX )

add_definitions( "-DTCAP_WHITE -D__GSM_ROUTE_PC__" )

include_directories(${INCLUDES})

add_library(zserver STATIC ${SRC_FILES})

install(TARGETS zserver DESTINATION ${LIB_DIR})

