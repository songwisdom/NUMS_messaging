file(GLOB SRC_FILES CONFIGURE_DEPENDS
	main.cpp init.cpp
)

set_source_files_properties( ${SRC_FILES} PROPERTIES LANGUAGE CXX )

#add_definitions( "-D__DEBUG__" )

include_directories( ${COMMON_INCLUDES} ./ )

link_libraries(
	"-Wl,--start-group"
	Common spdlog pthread
	"-Wl,--end-group"
)

add_executable(myapp ${SRC_FILES} )

install(PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/myapp DESTINATION ${BIN_DIR} )
