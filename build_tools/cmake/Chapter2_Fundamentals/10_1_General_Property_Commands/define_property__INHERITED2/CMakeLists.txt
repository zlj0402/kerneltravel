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

#[===========[subject1: 探究同级两个目录，其中一个目录 define_property(TARGET) 是否影响同级目录中的 target]===========]
add_subdirectory(attempts)
#[===========[subject1: 探究同级两个目录，其中一个目录 define_property(TARGET) 是否影响同级目录中的 target]===========]


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
@brief: 想探究同级目录中，在其中一个目录的 CMakeLists.txt define_property(TARGET)，
		是否会影响另一个同级目录下的 get_property(TARGET) 的属性；
@author: liangj.zhang
@date: 2026/09/17

@summary: 
	+ 不影响，直接看运行输出
	+ 结合两个例子（define_property__INHERITED、 define_property__INHERITED2），
		define_property() 只影响同一继承链上所有同类 entityType
]]