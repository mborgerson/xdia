include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}/../include
	${CMAKE_CURRENT_SOURCE_DIR}/../include/wine
	${CMAKE_CURRENT_SOURCE_DIR}/../include/dia
	${ICU_INCLUDE_DIRS}
	)

add_compile_options(
	${GLIB_CFLAGS_OTHER}
	-D__USE_GNU -D_GNU_SOURCE -D_POSIX_C_SOURCE=200112 -DWINE_UNICODE_NATIVE -DU_HAVE_STDINT_H
	-fshort-wchar
	-Wall
	-Wno-unknown-pragmas
	-Wno-trigraphs
	-Wno-changes-meaning
	)
	# -g -Og

set(SRC_COMMON
	pe-loader/pe-loader.c
	pe-loader/wc.c
	pe-loader/winapi/advapi32.c
	pe-loader/winapi/kernel32.c
	pe-loader/winapi/ole32.c
	pe-loader/winapi/oleaut32.c
	pe-loader/winapi/shlwapi.c
	)

# A build of dia2dump that calls natively into msdia140.dll
add_executable(dia2dump
	${SRC_COMMON}
	DIA2Dump/DIA2Dump.cpp
	DIA2Dump/PrintSymbol.cpp
	DIA2Dump/regs.cpp
	DIA2Dump/main.c
	DIA2Dump/objbase.c
	)
target_link_libraries(dia2dump
	${ICU_LIBRARIES}
	)

# xdia.exe loader
add_executable(xdialdr
	${SRC_COMMON}
	xdia-loader/main.c
	)
target_link_libraries(xdialdr
	${ICU_LIBRARIES}
	)

install(TARGETS xdialdr DESTINATION bin)
