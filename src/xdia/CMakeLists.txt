cmake_minimum_required(VERSION 3.15)

project(xdia)

add_compile_options(/MT)

include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}/../../include
	)

add_executable(xdia
	main.cpp
	PrintSymbol.cpp
	regs.cpp
	util.cpp
	)

install(TARGETS xdia DESTINATION bin)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/../../lib/msdia140.dll DESTINATION bin)
