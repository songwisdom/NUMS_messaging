file(GLOB SRC_FILES CONFIGURE_DEPENDS
	contentsConvert.c flk_iconv.c
)
set_source_files_properties( ${SRC_FILES}  PROPERTIES LANGUAGE CXX )

add_definitions( "-DTCAP_WHITE -D__GSM_ROUTE_PC__" )

include_directories(${INCLUDES})

add_library(sim_iconv STATIC ${SRC_FILES})

install(TARGETS sim_iconv DESTINATION ${LIB_DIR})



