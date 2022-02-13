cmake_minimum_required(VERSION 3.0)
project(
	klist 
	VERSION 1.0 
	DESCRIPTION "LIST array library"
	HOMEPAGE_URL ""
	LANGUAGES C
)


set (CMAKE_C_STANDARD 11)

add_library(klist STATIC klist.c)

add_executable(klist_test test.c)
target_link_libraries(klist_test klist)
