file(GLOB SRC_FILES CONFIGURE_DEPENDS
	mapDmaqi.c mapDmaqiEx.c mapDmaqiDebug.c LongMsg.c SpamMsg.c
)

add_definitions( "${COMPANY_TYPE}"  )

set_source_files_properties( ${SRC_FILES}  PROPERTIES LANGUAGE CXX )

include_directories(${COMMON_INCLUDES})

add_library(mapDmaqi STATIC ${SRC_FILES})

install(TARGETS mapDmaqi DESTINATION ${LIB_DIR})
