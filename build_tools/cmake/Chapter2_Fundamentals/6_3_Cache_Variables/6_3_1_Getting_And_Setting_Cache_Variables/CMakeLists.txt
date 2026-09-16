cmake_minimum_required(VERSION 3.25)
project(TestApp LANGUAGES CXX)

enable_testing()

message("PROJECT_SOURCE_DIR = ${PROJECT_SOURCE_DIR}")
message("CMAKE_SOURCE_DIR = ${CMAKE_SOURCE_DIR}")
message("CMAKE_INSTALL_PREFIX = ${CMAKE_INSTALL_PREFIX}")

#[=====================[ try cache variables ]=====================]

#[[测试 INTERVAL 前输出，进行对比]]
message("=== ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt")

message("before interval set")
message("var_with_interval = ${var_with_interval}")
message("")

#[[主要测试集中于 attempts 目录当中]]
add_subdirectory(attempts)

#[=====================[ try cache variables ]=====================]

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


#[=================================================[
@brief: 尝试 6.3.1 节中描述的部分 cache variable 特性
@author: liangj.zhang
@date: 2026/09/15

@content：
具体测试项，在 attempts 对下目录下有总结；
]=================================================]