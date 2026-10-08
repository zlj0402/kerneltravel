cmake_minimum_required(VERSION 3.15)
set(CMAKE_MESSAGE_CONTEXT_SHOW ON CACHE BOOL "Show message context")
list(APPEND CMAKE_MESSAGE_CONTEXT "main")
project(16_2_Target_Property_Commands LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
enable_testing()

message("PROJECT_SOURCE_DIR = ${PROJECT_SOURCE_DIR}")
message("CMAKE_SOURCE_DIR = ${CMAKE_SOURCE_DIR}")
message("CMAKE_INSTALL_PREFIX = ${CMAKE_INSTALL_PREFIX}")

message("=== ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt")
get_property(isMultiConfig GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)
# 检测是否为多配置生成器
if(isMultiConfig)
    message(STATUS "")
    message(STATUS "╔══════════════════════════════════════════╗")
    message(STATUS "║  Multi-config Generator Detected         ║")
    message(STATUS "╠══════════════════════════════════════════╣")
    message(STATUS "║ Generator: ${CMAKE_GENERATOR}")
    message(STATUS "║ Configs:   ${CMAKE_CONFIGURATION_TYPES}")
    message(STATUS "╠══════════════════════════════════════════╣")
    message(STATUS "║ Build: cmake --build . --config <Config> ║")
    message(STATUS "╚══════════════════════════════════════════╝")
    message(STATUS "")
else()
    # set default build type; if set before, there is no influence(cache variable characteristic)
    # avoid 'empty' build type
    message(STATUS "")
    message(STATUS "╔══════════════════════════════════════════╗")
    message(STATUS "║  Single-config Generator Detected        ║")
    message(STATUS "╠══════════════════════════════════════════╣")
    message(STATUS "║ Generator:  ${CMAKE_GENERATOR}")
    message(STATUS "║ Build Type: ${CMAKE_BUILD_TYPE}")
    message(STATUS "╠══════════════════════════════════════════╣")
    message(STATUS "║ Build: cmake --build .                   ║")
    message(STATUS "╚══════════════════════════════════════════╝")
    message(STATUS "")
endif()

#[=======================================[]=======================================]

# 只取消注释其中一行，看看区别
# add_library(Algo STATIC src/Algo.cpp)
# add_library(Algo OBJECT src/Algo.cpp)
add_library(Algo SHARED src/Algo.cpp)

add_library(AlgoImpl src/AlgoImpl.cpp)
add_library(Compute src/Compute.cpp)
add_executable(MyApp src/main.cpp)

# 设置 include 目录
target_include_directories(AlgoImpl PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/include)
# 相对目录，${CMAKE_CURRENT_SOURCE_DIR}/ =》 会被自动加上
# target_include_directories(AlgoImpl PRIVATE src/include)
target_include_directories(Compute PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/include)
target_include_directories(Algo PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/include)
# 注意：MyApp 不应该直接添加 include 目录
# - Compute.h 能找到：因为 Compute 是 Algo 的 PUBLIC 依赖，include 目录会传递
# - AlgoImpl.h 找不到：因为 AlgoImpl 是 Algo 的 PRIVATE 依赖，include 目录不会传递

# Algo 的依赖关系
target_link_libraries(Algo
    PUBLIC
        Compute      # PUBLIC: Algo 自己用，消费者（MyApp）也能用
    PRIVATE
        AlgoImpl     # PRIVATE: 只有 Algo 自己用，消费者看不到
)

# MyApp 的依赖关系
target_link_libraries(MyApp
    PRIVATE
        Algo
)

#[==========[]==========]
add_subdirectory(attempts)

install(TARGETS MyApp Algo)

add_test(NAME myAppTest COMMAND MyApp)

add_custom_target(clean_all
	${CMAKE_COMMAND} -E remove_directory ${CMAKE_SOURCE_DIR}/output
	COMMAND ${CMAKE_COMMAND} -E remove_directory ${PROJECT_SOURCE_DIR}/build
	COMMAND ${CMAKE_COMMAND} -E remove_directory ${PROJECT_SOURCE_DIR}/Testing
	COMMENT "Cleaning output directory..."
)

# 在构建时打印 MyApp 解析后的 INCLUDE_DIRECTORIES
add_custom_command(TARGET MyApp POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E echo "MyApp INCLUDE_DIRECTORIES = $<TARGET_PROPERTY:MyApp,INCLUDE_DIRECTORIES>"
    COMMAND ${CMAKE_COMMAND} -E echo "MyApp INTERFACE_INCLUDE_DIRECTORIES = $<TARGET_PROPERTY:MyApp,INTERFACE_LINK_LIBRARIES>"
)

#[[
@brief: 从 16_2_Target_Property_Commands copy 过来的例子，可以作为该章节内通用的例子；
@author: liangj.zhang
@date: 2026/10/08
]]