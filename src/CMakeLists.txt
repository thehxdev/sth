include(MimallocImport.cmake)

add_library(
	sth STATIC
	memory.c
)

target_compile_options(
	sth
	PRIVATE
	"$<BUILD_INTERFACE:-W3>"
)

target_compile_features(sth PRIVATE c_std_11)
target_include_directories(sth PUBLIC "${STH_INCLUDE_DIR}")
target_link_libraries(sth PRIVATE "mimalloc-static")
