set(
	sth_public_headers
	${STH_PUBLIC_HEADERS_DIR}/sth/malloc.h
	${STH_PUBLIC_HEADERS_DIR}/sth/debug.h
	${STH_PUBLIC_HEADERS_DIR}/sth/string.h
	${STH_PUBLIC_HEADERS_DIR}/sth/mutex.h
)

set(
	sth_source_files
	malloc.c
	string.c
)

add_library(
	sth
	${sth_source_files}
	${sth_public_headers}
)

target_compile_options(
	sth
	PRIVATE
	"$<BUILD_INTERFACE:-Wall;-Wextra>"
	# "$<BUILD_INTERFACE:$<${STH_WITH_MIMALLOC}:-fno-builtin-malloc>>"
)

target_compile_features(sth PRIVATE c_std_11)
target_include_directories(
	sth
	PUBLIC
	"$<INSTALL_INTERFACE:include>"
	"$<BUILD_INTERFACE:${STH_PUBLIC_HEADERS_DIR}>"
)
# target_include_directories(sth PUBLIC "${STH_INCLUDE_DIR}")

set_target_properties(
	sth PROPERTIES
	PUBLIC_HEADER "${sth_public_headers}"
	VERSION ${PROJECT_VERSION}
)

file(COPY ${sth_public_headers} DESTINATION "include/sth")

if (STH_WITH_MIMALLOC)
	include(MimallocImport)
	target_link_libraries(sth PRIVATE "$<BUILD_INTERFACE:mimalloc-obj>")
	target_compile_definitions(sth PRIVATE STH_WITH_MIMALLOC=1)
endif()

install(
	TARGETS sth EXPORT sth
	ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
	LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
	RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
	PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/sth"
)
