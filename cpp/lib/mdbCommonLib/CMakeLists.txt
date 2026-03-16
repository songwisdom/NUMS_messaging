file(GLOB SRC_FILES CONFIGURE_DEPENDS
	mdbErrStr.c mdbDmaqiUtil.c mdbDmaqiEx.c mdbQmi.c
	mdbDirecetInsert.c mdbProcStat.c StatRecUtil.c mdbTextConfig.c StatEsmeQ.c mdbAdr.c
)

add_definitions( "${COMPANY_TYPE}"  )

set_source_files_properties( ${SRC_FILES}  PROPERTIES LANGUAGE CXX )

include_directories(${COMMON_INCLUDES})

add_library(mdbCom STATIC ${SRC_FILES})

install(TARGETS mdbCom DESTINATION ${LIB_DIR})
