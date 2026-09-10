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
	src/include/utils.hpp
)
target_include_directories(Utils PUBLIC ${CMAKE_SOURCE_DIR}/src/include)
target_link_libraries(mainTool PRIVATE Utils)


install(TARGETS mainTool Utils)
# install(DIRECTORY src/include DESTINATION include)
install(DIRECTORY src/include/ DESTINATION include)

add_test(NAME mainToolTest COMMAND mainTool)

add_custom_target(clean_all
	${CMAKE_COMMAND} -E remove_directory ${CMAKE_SOURCE_DIR}/output
	COMMAND ${CMAKE_COMMAND} -E remove_directory ${PROJECT_SOURCE_DIR}/build
	COMMAND ${CMAKE_COMMAND} -E remove_directory ${PROJECT_SOURCE_DIR}/Testing
	COMMENT "Cleaning output directory..."
)

#[[ 运行方式：

1. 命令行（已经有 build 需要提前删除）：
cmake --preset debug
cmake --build --preset debug-build
cmake --install build --prefix output

2. 脚本(git bash 中运行与 windows)： 个人练习推荐
./build.sh -c -i

3. vscode 中运行：
.vscode 下面的 setting.json [c_cpp_properties.json] 已经配置好的话（见 Notion CMake/professional CMake/example/项目根目录下的 .vscode），
点击最下面 Build 按钮
]]

#[=======================================================[
@brief: 探究 install(DIRECTORY dir DESTINATION <dest.>) 和
			 install(DIRECTORY dir/ DESTINATION <dest.>) 的差别
@author: liangj.zhang
@date: 2026/09/09

@content:
	1. install(DIRECTORY src/include DESTINATION include)
	2. install(DIRECTORY src/include/ DESTINATION include)
	上面两句的差别，有 '/' 和没有 '/' 的差别：

	1. 没有 '/': install(DIRECTORY src/include DESTINATION include)
	output: 

	MINGW64 /d/Codes/zljgit/kerneltravel/build_tools/cmake/Chapter1_Getting_Started/5_2_Installing/install_DIRECTORY_trailing_difference/output (main)
	$ cmd //c "tree /f"
	卷 软件 的文件夹 PATH 列表
	卷序列号为 000001C0 6B78:46D0
	D:.
	├─bin
	│      libUtils.dll
	│      mainTool.exe
	│
	├─include
	│  └─include
	│          utils.hpp
	│
	└─lib
			libUtils.dll.a

	2. 有 '/'：install(DIRECTORY src/include/ DESTINATION include)
	output:

	MINGW64 /d/Codes/zljgit/kerneltravel/build_tools/cmake/Chapter1_Getting_Started/5_2_Installing/install_DIRECTORY_trailing_difference/output (main)
	$ cmd //c "tree /f"
	卷 软件 的文件夹 PATH 列表
	卷序列号为 6B78-46D0
	D:.
	├─bin
	│      libUtils.dll
	│      mainTool.exe
	│      
	├─include
	│      utils.hpp
	│      
	└─lib
			libUtils.dll.a

@summary:
	+ 就是会不会将目录也 copy 过去的区别；
	+ 有 '/' 的情况，似乎更符合大多数使用的场景
]=======================================================]