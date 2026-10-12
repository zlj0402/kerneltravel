cmake_minimum_required(VERSION 3.25)
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

# 关联 Algo 的 public 头文件
target_sources(Algo PUBLIC FILE_SET algo_pub
    TYPE HEADERS
    BASE_DIRS src/include
    FILES
        src/include/Algo.h
)

# 关联 Algo 的 private 头文件
# 注释掉，因为不会被安装，也不会传递给消费者；
# target_sources(Algo PRIVATE FILE_SET algo_pri
#     TYPE HEADERS
#     BASE_DIRS src/include
#     FILES
#         src/include/Compute.h
#         src/include/AlgoImpl.h
# )

# MyApp 的依赖关系
target_link_libraries(MyApp
    PRIVATE
        Algo
)

#[==========[]==========]
add_subdirectory(attempts)

install(TARGETS Algo FILE_SET algo_pub)
install(TARGETS MyApp RUNTIME DESTINATION bin)

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
@brief: 使用 $<BUILD_INTERFACE:...> $<INSTALL_INTERFACE:...> 来只在对应阶段赋予相应的属性值； &&
        并在对应阶段，输出对应属性的值，比如 INCLUDE_DIRECTORIES
@author: liangj.zhang
@date: 2026/10/10

@summary: -- about installing library for other projects using
+ 先说点其他知识点：
    1. 编译单元（.c/.cpp）#include 的那些头文件，在预处理阶段会包含进 .c/.cpp 当中；
        不过，是不会将头文件声明的那些接口的实现，在预处理阶段直接包含进来（应该是通过后面的链接阶段）；
+ 正题：
    1. 安装 exe 和 lib 到别的机器上的系统路径下，exe 是不需要额外的支持的（不需要提供头文件），它是最终产物，不是给其他项目用的。
        对于 lib，虽然它是完全体，各个头文件声明的接口，的实现都被包含进来了；
        但是，别的项目想要知道，这个库能提供哪些功能，还是得通过头文件，将其暴漏出来；
        比如这里的 Algo lib，如果 lib 有内容使用的接口，这些接口的头文件可以不用 install，只 install 需要提供接口所在头文件；
        --
        库提供出来，必须至少会有一个头文件一起安装；
]]