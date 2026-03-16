file(GLOB SRC_FILES CONFIGURE_DEPENDS
    Worker.cpp
)
set_source_files_properties( ${SRC_FILES}  PROPERTIES LANGUAGE CXX )

add_definitions( "-DTCAP_WHITE -D__GSM_ROUTE_PC__" )

include_directories(${INCLUDES})

add_library(prac_sync STATIC ${SRC_FILES})

install(TARGETS prac_sync DESTINATION ${LIB_DIR})

