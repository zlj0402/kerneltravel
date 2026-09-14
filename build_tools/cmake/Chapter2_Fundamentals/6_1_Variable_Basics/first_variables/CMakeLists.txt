cmake_minimum_required(VERSION 3.25)
project(TestApp LANGUAGES CXX)

enable_testing()

message("PROJECT_SOURCE_DIR = ${PROJECT_SOURCE_DIR}")
message("CMAKE_SOURCE_DIR = ${CMAKE_SOURCE_DIR}")
message("CMAKE_CURRENT_SOURCE_DIR = ${CMAKE_CURRENT_SOURCE_DIR}")
message("CMAKE_INSTALL_PREFIX = ${CMAKE_INSTALL_PREFIX}")


#[===============================[ try variable features]===============================]
add_subdirectory(attempts)

#[===============================[ try variable features]===============================]


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

#[=====================================[
@brief: 尝试变量名是否是大小写敏感的（是）；
		尝试 "" 的换行；
		尝试 [[ ]] 的换行，不会变量扩展，转义等；
@author: liangj.zhang
@date: 2026/09/14

@content:
1. attempts/case_sensitive/CMakeLists.txt
2. attempts/multiLine/CMakeLists.txt
3. attempts/bracket_syntax/CMakeLists.txt

结论已经在上面三个文件中输出了，也可以运行查看；

@output:

[cmake] === D:/Codes/zljgit/kerneltravel/build_tools/cmake/Chapter2_Fundamentals/6_1_Variable_Basics/first_variables/attempts/case_sensitive/CMakeLists.txt
[cmake] foo = ab
[cmake] Foo = cd
[cmake] verified: 变量名是 case_sensitive 的
[cmake] 
[cmake] === D:/Codes/zljgit/kerneltravel/build_tools/cmake/Chapter2_Fundamentals/6_1_Variable_Basics/first_variables/attempts/multiLine/CMakeLists.txt
[cmake] multiLine = First Line goes here
[cmake] Second Line with a "quoted" word
[cmake] verified: 字符串值付给变量时，可以换行，且不需要手动添加 \n 换行符
[cmake] 
[cmake] === D:/Codes/zljgit/kerneltravel/build_tools/cmake/Chapter2_Fundamentals/6_1_Variable_Basics/first_variables/attempts/bracket_syntax/CMakeLists.txt
[cmake] multiLine = First Line
[cmake] Second Line\n
[cmake] ${myVar}
[cmake] 
[cmake] multiLine2 = First Line
[cmake] Second Line\n
[cmake] ${myVar}
[cmake] -- Configuring done (0.3s)
[cmake] 
[cmake] verified:
[cmake] 1. 字符串直接换行，不需要手动加 \n 换行符
[cmake] 2. 转义符号不起作用
[cmake] 3. 变量替换不起作用
[cmake] 4. [[ 后面第一个换行符会忽略掉（不是手动的换行符，隐藏的；即，[[ 之后直接换行了）
[cmake] 
[cmake] 即，如书中所说：如果开头方括号后紧跟换行符，则忽略第一个换行符，但后续换行符不会。此外，不会对方括号内的内容进行进一步转换（即没有变量替换或转义）
]=====================================]