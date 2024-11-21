# Comand line set args
# "cmake . -D var:BOOL=ON -D var2... "-- rewrites cache 
# to delete "cmake -U varName"
# cmake_minimum_required(VERSION major.minor[.patch[.tweak])
cmake_minimum_required(VERSION 3.5)

set(RELEASE YES)

#project(projName
#    VERSION major.minor[.patch[.tweak]
#    DESCRIPTION ""
#    LANGUAGES languageName languageName ...)
project(console_parser
    VERSION 1.0.0
    DESCRIPTION ""
    LANGUAGES CXX)

list(
  APPEND
    PROJECT_INCLUDE_DIRECTORIES
    "${console_parser_SOURCE_DIR}/vcpkg_installed/x64-windows/include"
  )
include_directories(${PROJECT_INCLUDE_DIRECTORIES})

  list(
  APPEND
    LINK_DIRECTORIES
    "${console_parser_SOURCE_DIR}/vcpkg_installed/x64-windows/lib"
  )
include_directories(${LINK_DIRECTORIES})

# Target to create executable
# add_executable(targetName source1...[sfiles]) * n with different targetName
set(TARGET alt_parser)
add_executable(${TARGET} cpp_ws.cpp)

#find_file(lboostsys REQUIRED "boost_system-vc140-mt.lib" "${websocket_bridge_SOURCE_DIR}/vcpkg_installed/x64-windows/lib")
target_link_libraries(${TARGET} PUBLIC "${console_parser_SOURCE_DIR}/vcpkg_installed/x64-windows/lib/boost_system-vc140-mt.lib")

#find_file(lboostrand REQUIRED "boost_random-vc140-mt.lib" "${websocket_bridge_SOURCE_DIR}/vcpkg_installed/x64-windows/lib")
target_link_libraries(${TARGET} PUBLIC "${console_parser_SOURCE_DIR}/vcpkg_installed/x64-windows/lib/boost_random-vc140-mt.lib")
#target_link_libraries(ws_bridge PUBLIC "${websocket_bridge_SOURCE_DIR}/vcpkg_installed/x64-windows/lib/icuuc.lib")
#target_link_libraries(ws_bridge PUBLIC "${websocket_bridge_SOURCE_DIR}/vcpkg_installed/x64-windows/lib/icuio.lib")
#target_link_libraries(ws_bridge PUBLIC "${websocket_bridge_SOURCE_DIR}/vcpkg_installed/x64-windows/lib/icudt.lib")
#target_link_libraries(ws_bridge PUBLIC "${websocket_bridge_SOURCE_DIR}/vcpkg_installed/x64-windows/lib/boost_locale-vc140-mt.lib")


if(RELEASE)
    set(RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/release/")
else()
    set(RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/test/")
endif()
set(EXECUTABLE_OUTPUT_PATH ${RUNTIME_OUTPUT_DIRECTORY})

file(COPY "${console_parser_SOURCE_DIR}/vcpkg_installed/x64-windows/bin/boost_system-vc143-mt-x64-1_83.dll" DESTINATION ${RUNTIME_OUTPUT_DIRECTORY})
file(COPY "${console_parser_SOURCE_DIR}/vcpkg_installed/x64-windows/bin/boost_random-vc143-mt-x64-1_83.dll" DESTINATION ${RUNTIME_OUTPUT_DIRECTORY})
#file(COPY "${websocket_bridge_SOURCE_DIR}/vcpkg_installed/x64-windows/bin/boost_locale-vc143-mt-x64-1_83.dll" DESTINATION ${RUNTIME_OUTPUT_DIRECTORY})

find_library(lcurl libcurl "${console_parser_SOURCE_DIR}/vcpkg_installed/x64-windows/lib")

file(COPY "${console_parser_SOURCE_DIR}/vcpkg_installed/x64-windows/bin/libcurl.dll" DESTINATION ${RUNTIME_OUTPUT_DIRECTORY})
#file(COPY "${websocket_bridge_SOURCE_DIR}/vcpkg_installed/x64-windows/bin/icuuc73.dll" DESTINATION ${RUNTIME_OUTPUT_DIRECTORY})
#file(COPY "${websocket_bridge_SOURCE_DIR}/vcpkg_installed/x64-windows/bin/icuio73.dll" DESTINATION ${RUNTIME_OUTPUT_DIRECTORY})
#file(COPY "${websocket_bridge_SOURCE_DIR}/vcpkg_installed/x64-windows/bin/icudt73.dll" DESTINATION ${RUNTIME_OUTPUT_DIRECTORY})

target_link_libraries(${TARGET} PUBLIC ${lcurl})

# extended
# add_executable(targetName {WIN32|MACOSX_BUNDLE} {EXCLUDE_FROM_ALL} source1...[sfiles]) * n with different targetName

# Target to create library
# add_library(targetName {STATIC|SHARED(dynamic_lib)|MODULE(dynamic_lib for running include)} {EXCLUDE_FROM_ALL} source1 etc)
#add_library(ws_bridgelib cpp_wslib.cpp)


# set(varName value {PARENT_SCOPE}) - set variable as string "" if contains whitespaces by default ';' as sparator
# set(BUILD_SHARED_LIBS YES) - set variable
# or in command in console cmake -DBUILD_SHARED_LIBS=YES /p/t/source
# get var by ${varName}
#set(mssg Hallo)
#message(${mssg})
#set(${mssg} Buy)
#message("${Hallo}")
#message("${qq}") # will be empty if undeclared
#message("$ENV{NUMBER_OF_PROCESSORS}") # get enviermental|system variables

# target_link_libraries(targetName <PRIVATE|PUBLIC|INTERFACE> item1 [item2 ...])
# target_link_libraries(ws_bridge PRIVATE ws_bridgelib)
# PRIVATE seen only with the explicit link
# PUBLIC both libs should be included in the target
# INTERFACE for header library

#  Cashed variables
# usage as normal ${var}
# set(cashedVar value1 CACHE type "docstringofvar" {FORCE}) FORCE to rewrite var or to ?
set(idiot TRUE CACHE BOOL "you are")
# Types:
# BOOL ON/OFF
# FILEPATH 
# STRING
# INTERNAL
option(optTrue helpString True) # create bool var
# Normal var is first in line if names are the same exept first run without cache


