cmake_minimum_required(VERSION 3.25)
project(TestApp LANGUAGES CXX)

enable_testing()

message("PROJECT_SOURCE_DIR = ${PROJECT_SOURCE_DIR}")
message("CMAKE_SOURCE_DIR = ${CMAKE_SOURCE_DIR}")
message("CMAKE_INSTALL_PREFIX = ${CMAKE_INSTALL_PREFIX}")


#[=============================[subject1: 探究 define_property() 的影响范围：同文件中]=============================]
#[=======================[]=======================]
set_property(DIRECTORY PROPERTY MY_THING HiThere)
#[=======================[]=======================]

add_executable(mainTool
	src/main.cpp
)

#[=======================[]=======================]
message("=== ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt")
define_property(TARGET PROPERTY MY_THING INHERITED)
#[=======================[]=======================]

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

#[=========================[]=========================]
get_property(result TARGET mainTool PROPERTY MY_THING)
message("mainTool MY_THING result = ${result}")

get_property(result2 TARGET Utils PROPERTY MY_THING)
message("Utils MY_THING result = ${result2}")
#[=========================[]=========================]
#[=============================[subject1: 探究 define_property() 的影响范围：同文件中]=============================]

#[=============================[subject2: 探究 define_property() 的影响范围：不同文件中]=============================]
add_subdirectory(attempts)
#[=============================[subject2: 探究 define_property() 的影响范围：不同文件中]=============================]

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
@brief: 探究 define_property() 的影响范围
@author: liangj.zhang
@date: 2026/09/17

@output: --subject1
	[cmake] === D:/Codes/zljgit/kerneltravel/build_tools/cmake/Chapter2_Fundamentals/10_1_General_Property_Commands/define_property__INHERITED/CMakeLists.txt
	[cmake] mainTool MY_THING result = HiThere
	[cmake] Utils MY_THING result = HiThere

@summary: --subject1(CMakeLists.txt)，同文件作用域中
	不论 target 是定义在 define_property(TARGET) 之前还是之后，
		这里 mainTool 是定义在 define_property() 之前
			Utils 是定义在 define_property() 之后
	get_property(TARGET) 都能在对应的 target 身上找到这个 property 的值；

@output: --subject2（attempts/CMakeLists.txt），不同文件，父子关系目录中；
	[cmake] === D:/Codes/zljgit/kerneltravel/build_tools/cmake/Chapter2_Fundamentals/10_1_General_Property_Commands/define_property__INHERITED/attempts/CMakeLists.txt
	[cmake] mainTool2 MY_THING result = HiThere

@summary:
	在下级目录依旧能获得 target 的 MY_THING 属性；

@summary: --all
	能够继承的前提不要忘记：target 的该属性没有被赋过值；
]]