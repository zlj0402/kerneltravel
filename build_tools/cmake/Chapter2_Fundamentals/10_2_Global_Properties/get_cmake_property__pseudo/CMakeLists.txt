cmake_minimum_required(VERSION 3.25)
project(TestApp LANGUAGES CXX)

enable_testing()

message("PROJECT_SOURCE_DIR = ${PROJECT_SOURCE_DIR}")
message("CMAKE_SOURCE_DIR = ${CMAKE_SOURCE_DIR}")
message("CMAKE_INSTALL_PREFIX = ${CMAKE_INSTALL_PREFIX}")

#[=========================[subject1: get_cmake_property() to get global pseudo property]=========================]
message("=== ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt")
get_cmake_property(resultVar MACROS)
message("global pseudo MACROS: 
	${resultVar}
")
unset(resultVar)

get_cmake_property(resultVar CACHE_VARIABLES)
message("global pseudo CACHE_VARIABLES:
	${resultVar}
")
unset(resultVar)
#[=========================[subject1: get_cmake_property() to get global pseudo property]=========================]

#[=========================[subject2: get_cmake_property() to get global property]=========================]
get_cmake_property(resultVar PACKAGES_FOUND)
message("global PACKAGES_FOUND: 
	${resultVar}
")
unset(resultVar)

get_cmake_property(resultVar RULE_LAUNCH_COMPILE)
message("global RULE_LAUNCH_COMPILE:
	${resultVar}
")
unset(resultVar)
#[=========================[subject2: get_cmake_property() to get global property]=========================]


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

#[=========================[subject2: get_cmake_property() to get global COMPONENTS property]=========================]
get_cmake_property(resultVar COMPONENTS)
message("global COMPONENTS: 
	${resultVar}
")
unset(resultVar)
#[=========================[subject2: get_cmake_property() to get global COMPONENTS property]=========================]


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
@brief: 
]]