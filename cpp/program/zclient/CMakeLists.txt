file(GLOB SRC_FILES CONFIGURE_DEPENDS
	main.cpp
)

set_source_files_properties( ${SRC_FILES} PROPERTIES LANGUAGE CXX )

#add_definitions( "-D__DEBUG__" )

include_directories( ${COMMON_INCLUDES} ./ )

link_libraries(
	"-Wl,--start-group"
	Common spdlog pthread zclient zmq
	"-Wl,--end-group"
)

add_executable(zclient_bin ${SRC_FILES} )


install(PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/zclient DESTINATION ${BIN_DIR} )
