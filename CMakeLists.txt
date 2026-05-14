# CMakeList.txt : CMake project for Paster-plus-plus, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.10)

project ("Paster-plus-plus" VERSION 1.0 LANGUAGES C CXX RC)

message(STATUS "Source directory: ${CMAKE_SOURCE_DIR}")
message(STATUS "Build directory: ${CMAKE_BINARY_DIR}")

if (NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

#set(TOOLCHAIN_PREFIX x86_64-w64-mingw32-g++)
IF (MINGW)
	message(status: " ** MINGW detected.. **")
	set(CMAKE_CXX_FLAGS "-municode -mwindows -static ${CMAKE_CXX_FLAGS}")
ENDIF()

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
include_directories(${CMAKE_SOURCE_DIR}/src)
set (SOURCES src/paster-plus-plus.cpp)
set (HEADERS src/paster-plus-plus.h)
set (RESOURCES src/paster-plus-plus.rc)
# Add source to this project's executable.
add_executable (Paster-plus-plus WIN32 ${SOURCES} ${HEADERS} ${RESOURCES})

# TODO: Add tests and install targets if needed.
