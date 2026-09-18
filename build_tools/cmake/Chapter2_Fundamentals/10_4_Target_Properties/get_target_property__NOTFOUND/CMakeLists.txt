cmake_minimum_required(VERSION 3.25)
project(TestApp LANGUAGES CXX)

enable_testing()

message("PROJECT_SOURCE_DIR = ${PROJECT_SOURCE_DIR}")
message("CMAKE_SOURCE_DIR = ${CMAKE_SOURCE_DIR}")
message("CMAKE_INSTALL_PREFIX = ${CMAKE_INSTALL_PREFIX}")


add_executable(mainTool
	src/main.cpp
)
add_library(Utils SHARED
	src/utils.cpp
)
target_sources(Utils PUBLIC FILE_SET utils # 3.23+
	TYPE HEADERS
	BASE_DIRS src/include
	FILES
		src/include/utils.hpp
)
target_link_libraries(mainTool PRIVATE Utils)

#[=======================[subject1: 就想探究一下 get_target_property() 没有该属性值时，-NOTFOUND 前面是变量名还是属性名称]=======================]
get_target_property(abcdef Utils VERSION)
message("Utils lib version: 
	${abcdef}
")
unset(abcdef)
#[=======================[subject1: 就想探究一下 get_target_property() 没有该属性值时，-NOTFOUND 前面是变量名还是属性名称]=======================]

install(TARGETS mainTool Utils)

add_test(NAME mainToolTest COMMAND mainTool)

add_custom_target(clean_all
	${CMAKE_COMMAND} -E remove_directory ${CMAKE_SOURCE_DIR}/output
	COMMAND ${CMAKE_COMMAND} -E remove_directory ${PROJECT_SOURCE_DIR}/build
	COMMAND ${CMAKE_COMMAND} -E remove_directory ${PROJECT_SOURCE_DIR}/Testing
	COMMENT "Cleaning output directory..."
)

#[[
cmake --preset debug
cmake --build --preset debug-build
cmake --install build --prefix output
]]

#[[
@brief: 就想探究一下 get_target_property() 没有该属性值时，-NOTFOUND 前面是变量名还是属性名称
@author: liangj.zhang
@date: 2026/09/18

@summary: -- subject1
	确实是 get_target_property() 当中我们给的变量名，不是属性名；
	其次，注意，像 get_property() 和 get_directory_property() 这些方法没有找到属性时，值是空字符串；
]]
