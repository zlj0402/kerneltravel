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

#[=========================[subject1: get other directory property]=========================]
message("=== ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt")

get_directory_property(resultVar DIRECTORY attempts LABLES)
message("subdirectory attempts LABELS property: 
	${resultVar}
")
unset(resultVar)

add_subdirectory(attempts)
get_directory_property(resultVar DIRECTORY attempts LABLES)
message("subdirectory attempts LABELS property: 
	${resultVar}
")
unset(resultVar)
#[=========================[subject1: get other directory property]=========================]

#[=========================[subject2: get current directory property]=========================]
get_directory_property(myThing MY_THING)
if(myThing STREQUAL "")
	message(WARNING "myThing directory property is empty or not set")
endif()
#[=========================[subject2: get current directory property]=========================]


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
@brief: 使用 get_directory_property() 获取当前目录指定属性的值，和当前目录属性的值；
@author: liangj.zhang
@date: 2026/09/18

@summary: -- subject1
	get_directory_property() 只能获得 cmake 当前已经遇到过的目录；
	这里特意在 add_subdirectory(attempts) 前后加了输出；前面的输出，还提示了错误，因为运行到那里，还没有遇到过 attempts 目录；
]]