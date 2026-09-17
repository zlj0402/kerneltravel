cmake_minimum_required(VERSION 3.25)
project(TestApp LANGUAGES CXX)

enable_testing()

message("PROJECT_SOURCE_DIR = ${PROJECT_SOURCE_DIR}")
message("CMAKE_SOURCE_DIR = ${CMAKE_SOURCE_DIR}")
message("CMAKE_INSTALL_PREFIX = ${CMAKE_INSTALL_PREFIX}")

#[======================================[subject1: 多次一举的方式 get 变量值（因为可以直接 ${}）]======================================]
message("=== ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt")
get_property(result VARIABLE PROPERTY CMAKE_SOURCE_DIR)
message("get_property() way to get CMAKE_SOURCE_DIR, result: 
	${result}"
)
#[======================================[subject1: 多次一举的方式 get 变量值（因为可以直接 ${}）]======================================]

#[======================================[subject2: get_property(... DEFINED)]======================================]
get_property(is_defined VARIABLE PROPERTY CMAKE_SOURCE_DIR DEFINED)
message("get_property(...DEFINED) way to get CMAKE_SOURCE_DIR defined or not, result:
	${is_defined}"
)
get_property(is_set VARIABLE PROPERTY CAMKE_SOURCE_DIR SET)
message("get_property(...SET) way to get CMAKE_SOURCE_DIR set or not, result:
	${is_set}"
)
unset(is_defined)
unset(is_set)
#[======================================[subject2: get_property(... DEFINED)]======================================]

#[======================================[
探索一下自定义变量的 SET 信息，因为上面 CMAKE 自带的变量，测试是没有 set 的
]======================================]
add_subdirectory(attempts)
#[======================================[]======================================]

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
@brief: 以多次一举的方式，来获得变量的值，就是想试试；
		试试 get_property(...DEFINED/SET) 的作用；
@author: liangj.zhang
@date: 2026/09/17

@summary:
	get_property(...SET) 对于 CMake 自带的变量，不会显示 SET 过的状态；（自定义变量 set 值为 1，CMake 自带变量值为 0）
	DEFINED 的状态，只能通过 define_property() 获得；（自定义变量，和 CMake 自带变量，都显示 defined 值为 0）
]]