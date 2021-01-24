# CMakeList.txt : CMake project for Paster-plus-plus, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("Paster-plus-plus")


# Add source to this project's executable.
add_executable (Paster-plus-plus WIN32 "src/paster-plus-plus.cpp" "src/paster-plus-plus.h")

# TODO: Add tests and install targets if needed.
