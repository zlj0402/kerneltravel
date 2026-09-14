cmake_minimum_required(VERSION 3.25)
project(TestApp LANGUAGES CXX)

enable_testing()

message("PROJECT_SOURCE_DIR = ${PROJECT_SOURCE_DIR}")
message("CMAKE_SOURCE_DIR = ${CMAKE_SOURCE_DIR}")
message("CMAKE_INSTALL_PREFIX = ${CMAKE_INSTALL_PREFIX}")

#[==========================[try set environment variable]==========================]
message("PATH = $ENV{PATH}")
set(ENV{PATH} "$ENV{PATH};D:\\Codes")
message("PATH = $ENV{PATH}")

add_subdirectory(attempts)

# 构建时，编译器看不到 PATH=...D:\Codes
add_custom_target(show_env
	COMMAND ${CMAKE_COMMAND} -E echo "Build: $ENV{PATH}"
    COMMAND ${CMAKE_COMMAND} -E echo "Build: $$ENV{PATH}"
)

#[==========================[try set environment variable]==========================]

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


#[=[
@brief: 尝试设置环境变量
@author: liangj.zhang
@date: 2026/09/14

@content:
	1. 在 top CMakeLists.txt 当中设置了环境变量，并且前后打印了一次；
	2. 在子目录中，打印了一次；

@output：

	PATH = D:\Tools\MSYS2\ucrt64\bin;...;D:\Git\usr\bin\core_perl
	PATH = D:\Tools\MSYS2\ucrt64\bin;...;D:\Git\usr\bin\core_perl;D:\Codes
	PATH = D:\Tools\MSYS2\ucrt64\bin;...;D:\Git\usr\bin\core_perl;D:\Codes

	cmake 命令是脚本运行的，运行之后，在同一个 git bash 终端当中运行 `echo ${PATH}`
	/d/Tools/MSYS2/ucrt64/bin:...:/usr/bin/core_perl

@summary:
	As soon as the CMake run is finished, the change to the environment variable is lost. In
	particular, the change to the environment variable will not be visible at build time.

	构建时，修改就不见了；

]=]