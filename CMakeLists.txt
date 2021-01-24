# CMakeList.txt : CMake project for Paster-plus-plus, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("Paster-plus-plus" VERSION 1.0)

set(CMAKE_CXX_COMPILER "/usr/bin/x86_64-w64-mingw32-g++")
set(CMAKE_CXX_FLAGS "-mwindows -static -municode")

# Add source to this project's executable.
add_executable (Paster-plus-plus WIN32 "src/paster-plus-plus.cpp" "src/paster-plus-plus.h")

# TODO: Add tests and install targets if needed.
