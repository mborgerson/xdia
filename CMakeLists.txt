cmake_minimum_required(VERSION 3.15)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

project(xdia)

set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
set(BUILD_SHARED_LIBS OFF)
set(CMAKE_EXE_LINKER_FLAGS "-static -Wl,-z,common-page-size=65536,-z,max-page-size=65536")

find_package(ICU REQUIRED COMPONENTS uc)

add_subdirectory(src)
