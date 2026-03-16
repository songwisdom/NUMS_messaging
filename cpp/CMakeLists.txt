cmake_minimum_required(VERSION 3.11)

project(src VERSION 1.0.0 LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_VERBOSE_MAKEFILE ON)

set(CMAKE_CXX_FLAGS     "-Wall -O2 -D_REENTRANT -D_GNU_SOURCE -DDUALHANDLER -DSPDLOG_COMPILED_LIB -DSPDLOG_NO_THREAD_ID -DSPDLOG_NO_TLS -DSPDLOG_PREVENT_CHILD_FD -DSTATISTICS_NEW -D_SMSC_SYSTEM_LINUX" )
set(CMAKE_C_FLAGS       "-Wall -O2 -D_REENTRANT -D_GNU_SOURCE -DDUALHANDLER -DSTATISTICS_NEW -D_SMSC_SYSTEM_LINUX" )

set(CMAKE_CXX_COMPILER "c++")
set(INC_DIR    ${CMAKE_SOURCE_DIR}/include)
set(INCLUDES   ${INC_DIR} ${INC_DIR}/Smpp3A ${INC_DIR}/c++ ${INC_DIR}/spdlog ${INC_DIR}/cpp ${INC_DIR}/stmt_mysql ${INC_DIR}/ksig7 /usr/include/mysql ${INC_DIR}/SimMap ${INC_DIR}/SimCommon ${INC_DIR}/smsc_inc ${INC_DIR}/smsc_inc/spdlog ${INC_DIR}/boost/1.83 ) 
set(INCLUDES   ${INC_DIR} ${INC_DIR}/newbee ${INC_DIR}/zserver ${INC_DIR}/zclient ${INC_DIR}/numsworker ${INC_DIR}/rapidjson ${INC_DIR}/prac_sync ${INC_DIR}/Smpp3A ${INC_DIR}/c++ ${INC_DIR}/spdlog ${INC_DIR}/cpp ${INC_DIR}/stmt_mysql ${INC_DIR}/ksig7 /usr/include/mysql ${INC_DIR}/SimMap ${INC_DIR}/SimCommon  ${INC_DIR}/boost/1.83 ) 

set(COMMON_INCLUDES ${INCLUDES})

set(LIB_DIR         "$ENV{USER_HOME}/lib")
set(SVC_LIB_DIR     "$ENV{SJH_SVC_HOME}/lib")
set(BIN_DIR         "$ENV{SJH_SVC_HOME}/bin")

set(BOOST_LDFLAGS   "-lboost_thread -lboost_system -lboost_filesystem -lpthread")
set(COMMON_LDFLAGS  "${BOOST_LDFLAGS} -lstdc++fs")
set(MYSQL_LDFLAGS   "-lmysqlclient")
set(COMPANY_TYPE    "-DSKT_VERSION")

link_directories(${LIB_DIR} /usr/lib64/mysql ${SVC_LIB_DIR}/mariadb )

add_subdirectory(lib)
add_subdirectory(program)

string(ASCII 27 Esc)
set(ColourReset "${Esc}[m")
set(ColourBold  "${Esc}[1m")
set(Red         "${Esc}[31m")
set(Green       "${Esc}[32m")
set(Yellow      "${Esc}[33m")
set(Blue        "${Esc}[34m")
set(Magenta     "${Esc}[35m")
set(Cyan        "${Esc}[36m")
set(White       "${Esc}[37m")
set(BoldRed     "${Esc}[1;31m")
set(BoldGreen   "${Esc}[1;32m")
set(BoldYellow  "${Esc}[1;33m")
set(BoldBlue    "${Esc}[1;34m")
set(BoldMagenta "${Esc}[1;35m")
set(BoldCyan    "${Esc}[1;36m")
set(BoldWhite   "${Esc}[1;37m")
message(STATUS "\t==================== Compiler C\t/\tC++ ====================")
message(STATUS " - ID       \t\t: ${CMAKE_C_COMPILER_ID}\t\t/\t${CMAKE_CXX_COMPILER_ID}")
message(STATUS " - Version  \t\t: ${CMAKE_C_COMPILER_VERSION}\t\t/\t${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS " - Path     \t\t: ${CMAKE_C_COMPILER} \t/\t${CMAKE_CXX_COMPILER}")
message(STATUS " - standard \t\t: ${CMAKE_C_STANDARD} \t\t/\t${CMAKE_CXX_STANDARD}")

message(STATUS " - LIB_DIR  \t\t: ${LIB_DIR} ")
message(STATUS " - BIN_DIR  \t\t:${Red}${ColourBold} ${BIN_DIR} ${ColourReset} ")
message(STATUS " - AGENT_BIN_DIR  \t:${Red}${ColourBold} ${AGENT_BIN_DIR} ${ColourReset}")

