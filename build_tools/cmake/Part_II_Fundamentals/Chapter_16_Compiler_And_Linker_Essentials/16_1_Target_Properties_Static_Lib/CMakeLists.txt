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
add_library(Algo STATIC src/Algo.cpp)
# add_library(Algo OBJECT src/Algo.cpp)
# add_library(Algo SHARED src/Algo.cpp)

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

#[================[subject1: 查看 STATIC_LIBRARY_OPTIONS 值是否作用到了 库管理工具 上]================]
set_target_properties(Algo PROPERTIES
    STATIC_LIBRARY_OPTIONS "-s"
)
#[================[subject1: 查看 STATIC_LIBRARY_OPTIONS 值是否作用到了 库管理工具 上]================]

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

#[==========================[subject1: 查看 STATIC_LIBRARY_OPTIONS 值是否作用到了 库管理工具 上]==========================]
add_custom_command(TARGET Algo POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E echo "Algo Compiler INCLUDE_DIRECTORIES = $<TARGET_PROPERTY:Algo,INCLUDE_DIRECTORIES>"
    COMMAND ${CMAKE_COMMAND} -E echo "Algo Compiler INTERFACE_INCLUDE_DIRECTORIES = $<TARGET_PROPERTY:Algo,INTERFACE_LINK_LIBRARIES>"
    COMMAND ${CMAKE_COMMAND} -E echo "Algo Compiler COMPILE_DEFINITIONS = $<TARGET_PROPERTY:Algo,COMPILE_DEFINITIONS>"
    COMMAND ${CMAKE_COMMAND} -E echo "Algo Compiler INTERFACE_COMPILE_DEFINITIONS = $<TARGET_PROPERTY:Algo,INTERFACE_COMPILE_DEFINITIONS>"
    COMMAND ${CMAKE_COMMAND} -E echo "Algo Compiler COMPILE_OPTIONS = $<TARGET_PROPERTY:Algo,COMPILE_OPTIONS>"
    COMMAND ${CMAKE_COMMAND} -E echo "Algo Compiler INTERFACE_COMPILE_OPTIONS = $<TARGET_PROPERTY:Algo,INTERFACE_COMPILE_OPTIONS>"
)

add_custom_command(TARGET Algo POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E echo "Algo Linker LINK_LIBRARIES = $<TARGET_PROPERTY:Algo,LINK_LIBRARIES>"
    COMMAND ${CMAKE_COMMAND} -E echo "Algo Linker INTERFACE_LINK_LIBRARIES = $<TARGET_PROPERTY:Algo,INTERFACE_LINK_LIBRARIES>"
    COMMAND ${CMAKE_COMMAND} -E echo "Algo Compiler STATIC_LIBRARY_OPTIONS = $<TARGET_PROPERTY:Algo,STATIC_LIBRARY_OPTIONS>"
    COMMAND ${CMAKE_COMMAND} -E echo "Algo Linker LINK_OPTIONS = $<TARGET_PROPERTY:Algo,LINK_OPTIONS>"
    COMMAND ${CMAKE_COMMAND} -E echo "Algo Linker INTERFACE_LINK_OPTIONS = $<TARGET_PROPERTY:Algo,INTERFACE_LINK_OPTIONS>"
    COMMAND ${CMAKE_COMMAND} -E echo "Algo Linker LINK_FLAGS = $<TARGET_PROPERTY:Algo,LINK_FLAGS>"
)
#[==========================[subject1: 查看 STATIC_LIBRARY_OPTIONS 值是否作用到了 库管理工具 上]==========================]

#[[
@brief: 输出 static target 与库管理工具的相关属性 STATIC_LIBRARY_OPTIONS，以及查看是否真的作用到 库管理工具 上
@author: liangj.zhang
@date: 2026/10/09

@why:
    pdf 当中说 STATIC_LIBRARY_OPTIONS 只对构建为静态库的目标有意义，将被用于库管理工具（librarian）或归档工具（archiver）；
    我就想知道是怎么被用于的 库管理工具；

    收获：
        1. 原来有库管理工具 ar.exe
        2. 它的作用时机
        3. 虽然更细节的部分还不够了解，但是被用于库管理工具，是怎样的，我是了解了；

@step:
    + 命令行上运行：./build.sh -c -g "MinGW Makefiles" -v
        默认的 Ninja，似乎不允许 custom command 中 echo 的内容包含单个 $ 符号，意思是需要转义，连续用两个 $$ 才可以；

@output: -- subject1
    D:\Tools\MSYS2\ucrt64\bin\ar.exe qc libAlgo.a -s CMakeFiles/Algo.dir/src/Algo.cpp.obj
        -s => 确实有作用到 ar.exe 工具上；（note: 库管理工具运行时间是，汇编之后，链接之前）
    Algo Compiler STATIC_LIBRARY_OPTIONS = -s => 这是 add_custom_command() 对于该属性的输出
        这行输出是在 上面命令的后面（不确定一定是否如此）
]]