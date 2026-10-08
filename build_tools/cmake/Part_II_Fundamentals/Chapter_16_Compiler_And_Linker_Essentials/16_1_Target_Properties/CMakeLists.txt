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

#[================[subject1: 输出非 static target 与编译器，链接器相关的属性]================]
add_custom_command(TARGET MyApp POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E echo "MyApp Compiler INCLUDE_DIRECTORIES = $<TARGET_PROPERTY:MyApp,INCLUDE_DIRECTORIES>"
    COMMAND ${CMAKE_COMMAND} -E echo "MyApp Compiler INTERFACE_INCLUDE_DIRECTORIES = $<TARGET_PROPERTY:MyApp,INTERFACE_LINK_LIBRARIES>"
    COMMAND ${CMAKE_COMMAND} -E echo "MyApp Compiler COMPILE_DEFINITIONS = $<TARGET_PROPERTY:MyApp,COMPILE_DEFINITIONS>"
    COMMAND ${CMAKE_COMMAND} -E echo "MyApp Compiler INTERFACE_COMPILE_DEFINITIONS = $<TARGET_PROPERTY:MyApp,INTERFACE_COMPILE_DEFINITIONS>"
    COMMAND ${CMAKE_COMMAND} -E echo "MyApp Compiler COMPILE_OPTIONS = $<TARGET_PROPERTY:MyApp,COMPILE_OPTIONS>"
    COMMAND ${CMAKE_COMMAND} -E echo "MyApp Compiler INTERFACE_COMPILE_OPTIONS = $<TARGET_PROPERTY:MyApp,INTERFACE_COMPILE_OPTIONS>"
)

add_custom_command(TARGET MyApp POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E echo "MyApp Linker LINK_LIBRARIES = $<TARGET_PROPERTY:MyApp,LINK_LIBRARIES>"
    COMMAND ${CMAKE_COMMAND} -E echo "MyApp Linker INTERFACE_LINK_LIBRARIES = $<TARGET_PROPERTY:MyApp,INTERFACE_LINK_LIBRARIES>"
    COMMAND ${CMAKE_COMMAND} -E echo "MyApp Linker LINK_OPTIONS = $<TARGET_PROPERTY:MyApp,LINK_OPTIONS>"
    COMMAND ${CMAKE_COMMAND} -E echo "MyApp Linker INTERFACE_LINK_OPTIONS = $<TARGET_PROPERTY:MyApp,INTERFACE_LINK_OPTIONS>"
    COMMAND ${CMAKE_COMMAND} -E echo "MyApp Linker LINK_FLAGS = $<TARGET_PROPERTY:MyApp,LINK_FLAGS>"
)

add_custom_command(TARGET Compute POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E echo "Compute Sources SOURCES = $<TARGET_PROPERTY:Compute,SOURCES>"
    COMMAND ${CMAKE_COMMAND} -E echo "Compute INTERFACE_SOURCES Sources INTERFACE_SOURCES = $<TARGET_PROPERTY:Compute,INTERFACE_SOURCES>"
)

add_custom_command(TARGET MyApp POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E echo "MyApp Sources SOURCES = $<TARGET_PROPERTY:MyApp,SOURCES>"
    COMMAND ${CMAKE_COMMAND} -E echo "MyApp Sources INTERFACE_SOURCES = $<TARGET_PROPERTY:MyApp,INTERFACE_SOURCES>"
)
#[================[subject1: 输出非 static target 与编译器，链接器相关的属性]================]

#[[
@brief: 输出非 static target 与编译器，链接器相关的属性
@author: liangj.zhang
@date: 2026/10/08

@summary: -- subject1
    如果命令行，没有带额外的标志，如 flag 等参数，
    除 LINK_LIBRARIES 会有相关的库被引入，其他属性皆没有输出；

@output: Output window in vscode ⭐
    [build] Compute Sources SOURCES = src/Compute.cpp
    [build] Compute INTERFACE_SOURCES Sources INTERFACE_SOURCES = 
    [build] [7/8  87% :: 3.100] Linking CXX shared library Debug\libAlgo.dll
    [build] [8/8 100% :: 4.584] Linking CXX executable Debug\MyApp.exe
    [build] MyApp Compiler INCLUDE_DIRECTORIES = D:/Codes/zljgit/kerneltravel/build_tools/cmake/Part_II_Fundamentals/Chapter_16_Compiler_And_Linker_Essentials/16_1_Target_Properties/src/include
    [build] MyApp Compiler INTERFACE_INCLUDE_DIRECTORIES = 
    [build] MyApp Compiler COMPILE_DEFINITIONS = 
    [build] MyApp Compiler INTERFACE_COMPILE_DEFINITIONS = 
    [build] MyApp Compiler COMPILE_OPTIONS = 
    [build] MyApp Compiler INTERFACE_COMPILE_OPTIONS = 
    [build] MyApp Linker LINK_LIBRARIES = Algo
    [build] MyApp Linker INTERFACE_LINK_LIBRARIES = 
    [build] MyApp Linker LINK_OPTIONS = 
    [build] MyApp Linker INTERFACE_LINK_OPTIONS = 
    [build] MyApp Linker LINK_FLAGS = 
    [build] MyApp Sources SOURCES = src/main.cpp
    [build] MyApp Sources INTERFACE_SOURCES = 
    [driver] Build completed: 00:00:04.760
    [build] Build finished with exit code 0

@phenomenon: Output window in vscode ⭐
    `Compute` target property 输出比 `MyApp` target property 输出更早，且一定如此；

@summary: Output window in vscode ⭐
    Claude 给出的原因：
        POST_BUILD 在目标链接完成后立刻执行，不是在全部构建完成后。
        Compute 先链接完成，所以它的输出先出现；MyApp 最后链接完成，所以它的输出最后出现。
]]