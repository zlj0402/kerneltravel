cmake_minimum_required(VERSION 3.25)
project(TestApp LANGUAGES CXX)

enable_testing()
include(CMakePrintHelpers)

message("PROJECT_SOURCE_DIR = ${PROJECT_SOURCE_DIR}")
message("CMAKE_SOURCE_DIR = ${CMAKE_SOURCE_DIR}")
message("CMAKE_CURRENT_SOURCE_DIR = ${CMAKE_CURRENT_SOURCE_DIR}")
message("CMAKE_INSTALL_PREFIX = ${CMAKE_INSTALL_PREFIX}")


add_executable(mainTool
	src/main.cpp
)


# add_library(Utils SHARED
# 	src/utils.cpp
# 	src/include/utils.hpp
# )
add_library(Utils)
target_sources(Utils PRIVATE src/utils.cpp)

# target_include_directories(Utils PUBLIC ${CMAKE_SOURCE_DIR}/src/include)
target_sources(Utils PUBLIC FILE_SET utils
#[=[ file sets，重要差别 API，以 PUBLIC/PRIVATE
target_sources(Utils PRIVATE FILE_SET utils
]=]
	TYPE HEADERS
	BASE_DIRS src/include
	FILES
		src/include/utils.hpp
)


target_link_libraries(mainTool PRIVATE Utils)

cmake_print_properties(
	TARGETS mainTool Utils
	PROPERTIES TYPE INCLUDE_DIRECTORIES INTERFACE_INCLUDE_DIRECTORIES
)
add_custom_target(printMainTool
    COMMENT "Result of mainTool directory properties:"
    COMMAND ${CMAKE_COMMAND} -E echo "INCLUDE_DIRECTORIES=$<TARGET_PROPERTY:mainTool,INCLUDE_DIRECTORIES>"
    COMMAND ${CMAKE_COMMAND} -E echo "INTERFACE_INCLUDE_DIRECTORIES=$<TARGET_PROPERTY:mainTool,INTERFACE_INCLUDE_DIRECTORIES>"
)

# install(TARGETS mainTool Utils)
install(TARGETS mainTool Utils FILE_SET utils) # 还能接 FILE_SET, 接上跟单独 install(DIRECTORY) fileSet 对象包含的路径一样；
# install(TARGETS mainTool Utils)
# install(DIRECTORY src/include/ DESTINATION include)

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


#[=======================[
@brief: 初步探究 target_sources 的使用（主要是 file sets）;
		&& <PRIVATE/PUBLIC/INTERFACE> 属性的传播链认识；
		&& install(TARGETS <> FILE_SET <>) 效果
@author: liangj.zhang
@date: 2026/09/11

@summary:
	```
	target_sources(targetName
		<PRIVATE|PUBLIC|INTERFACE>
			FILE_SET setName
			[TYPE fileType]
			[BASE_DIRS dir1 [dir2...]]
			[FILES file1 [file2...]]
			...
	)
	```

	+ file sets 的基本使用：
		1. 当 setName 跟 TYPE 后面的 fileType 相同时，TYPE 才可以省略；想法情况，则必须提供；
		2. FILES 后面文件的给出：
			a. 位于某个 base dir；dir 是什么，file 以 `<dir>/file` 给出;
			b. 非上面的路径，会被认为是，相对于调用 target_sources() 时的 CMAKE_CURRENT_SOURCE_DIR;
			c. $< 开头的路径（生成器表达式），相对于 target 的源目录（通常是 add_library/add_executable 所在的目录）；


	+ 为 target 添加 source files 的方式：
		1. add_library(Utils SHARED src/utils.cpp)
		2. 
		```
		target_sources(targetName
			<PRIVATE|PUBLIC|INTERFACE> file1 [file2 ...]
			[<PRIVATE|PUBLIC|INTERFACE> file3 [file4 ...]]
			...
		)
	```

@output: target_sources(Utils PRIVATE FILE_SET utils...)

	[build] D:\Codes\zljgit\kerneltravel\build_tools\cmake\Chapter1_Getting_Started\5_2_Installing\first_file_sets\src\main.cpp:1:10: fatal error: utils.hpp: No such file or directory
	[build]     1 | #include "utils.hpp"
	[build]       |          ^~~~~~~~~~~
	[build] compilation terminated.

	原因：
	Utils 库以 PRIVATE 方式指定了一些头文件包含进自己；
	内部效果就是 INCLUDE_DIRECTORIES（PRIVATE）属性添加了一些头文件，
	如果消费者（link 这个 Utils 库的 target）链接这个库，这些头文件，消费者是搜索不到的，
		尤其是 mainTool 当中使用到了 Utils 库当中的实现，找不到实现，就会报错，如上所示；

@summary:
	+ target_sources() 如果是用于 file sets
		a. 作为被外部使用，那 Utils 需要以 PUBLIC 的方式指定是哪些头文件；会被外部感知
		b. 被内部使用，那 Utils 需要以 PRIVATE 的方式指定是哪些头文件；不会被外部感知
		上面两个步骤是不相冲突的；
	+ target_sources() 还能用于添加 source files 到 target 当中，效果相当于
		add_libraries() + target_link_libraries()
		add_executable() + target_link_libraries()
		见上面的总结

@output:

	1. install(TARGETS mainTool Utils FILE_SET utils)
	MINGW64 /d/Codes/zljgit/kerneltravel/build_tools/cmake/Chapter1_Getting_Started/5_2_Installing/first_file_sets (main)
	$ cd output && cmd //c "tree /f"
	卷 软件 的文件夹 PATH 列表
	卷序列号为 6B78-46D0
	D:.
	├─bin
	│      mainTool.exe
	│
	├─include
	│      utils.hpp
	│
	└─lib
			libUtils.a

	2. install(TARGETS mainTool Utils)
	MINGW64 /d/Codes/zljgit/kerneltravel/build_tools/cmake/Chapter1_Getting_Started/5_2_Installing/first_file_sets (main)
	$ cd output && cmd //c "tree /f"
	卷 软件 的文件夹 PATH 列表
	卷序列号为 6B78-46D0
	D:.
	├─bin
	│      mainTool.exe
	│      
	└─lib
			libUtils.a

@summary:

include(TARGETS <> FILE_SET <>) 
target 后面接上 link 的 file set 对象，竟然可以做到等同单独 include(DIRECTORY) file sets对象(这里是 utils)涉及到的路径+文件
]=======================]