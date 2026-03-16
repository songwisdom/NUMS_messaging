file(GLOB SRC_FILES CONFIGURE_DEPENDS
	dataElement.cpp
	send_link.cpp
)
set_source_files_properties( ${SRC_FILES}  PROPERTIES LANGUAGE CXX )

add_definitions( "-DTCAP_WHITE -D__GSM_ROUTE_PC__" )

include_directories(${INCLUDES})

add_library(newbee STATIC ${SRC_FILES})

install(TARGETS newbee DESTINATION ${LIB_DIR})

