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
# 注意：MyApp 不应该直接添加 include 目录
# - Compute.h 能找到：因为 Compute 是 Algo 的 PUBLIC 依赖，include 目录会传递
# - AlgoImpl.h 找不到：因为 AlgoImpl 是 Algo 的 PRIVATE 依赖，include 目录不会传递

#[==================[action: install(EXPORT ...) 导出库时，需要将 PUBLIC 的依赖也要导出，这里直接改为 PRIVATE,省掉导出]==================]
# Algo 的依赖关系
target_link_libraries(Algo
    PRIVATE
        AlgoImpl
        Compute
)
#[==================[action: install(EXPORT ...) 导出库时，需要将 PUBLIC 的依赖也要导出，这里直接改为 PRIVATE,省掉导出]==================]

#[==================[subject1: 给 Algo 库的 INCLUDE_DIRECTORIES 赋值生成表达式，使该属性在构建，安装上下文，具备不同的值]==================]
# 1. 设置包含路径（编译时用）
target_include_directories(Algo PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/BUILD_ONLY>
    $<INSTALL_INTERFACE:include/INSTALL_ONLY>
)

# 2. 管理头文件集合（安装时用）
target_sources(Algo PUBLIC FILE_SET algo_pub
    TYPE HEADERS
    BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/src/include
    FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/include/Algo.h
)

# 导出 target，生成 CMake 配置文件
install(TARGETS Algo
    EXPORT AlgoTargets
    FILE_SET algo_pub DESTINATION include
)

# 导出配置文件
install(EXPORT AlgoTargets
    DESTINATION lib/cmake/Algo
)

# 构建时：显示 BUILD_INTERFACE 的值
add_custom_command(TARGET Algo POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E echo "=== BUILD PHASE ==="
    COMMAND ${CMAKE_COMMAND} -E echo "INCLUDE = $<TARGET_PROPERTY:Algo,INTERFACE_INCLUDE_DIRECTORIES>"
)
#[==================[subject1: 给 Algo 库的 INCLUDE_DIRECTORIES 赋值生成表达式，使该属性在构建，安装上下文，具备不同的值]==================]

#[========[action: Algo 库自己 private 方式包含 Compute，无法传递给 MyApp， 这里就让 MyApp 自己包含]========]
# MyApp 的依赖关系
target_link_libraries(MyApp
    PRIVATE
        Algo
        Compute
)
#[========[action: Algo 库自己 private 方式包含 Compute，无法传递给 MyApp， 这里就让 MyApp 自己包含]========]

#[==========[]==========]
add_subdirectory(attempts)

# install(TARGETS Algo FILE_SET algo_pub)
install(TARGETS MyApp RUNTIME DESTINATION bin)

add_test(NAME myAppTest COMMAND MyApp)

add_custom_target(clean_all
	${CMAKE_COMMAND} -E remove_directory ${CMAKE_SOURCE_DIR}/output
	COMMAND ${CMAKE_COMMAND} -E remove_directory ${PROJECT_SOURCE_DIR}/build
	COMMAND ${CMAKE_COMMAND} -E remove_directory ${PROJECT_SOURCE_DIR}/Testing
	COMMENT "Cleaning output directory..."
)

#[=========================[note: install(CODE ...) 的方式，无法正确输出 生成表达式的结果]=========================]
# 安装时：显示 INSTALL_INTERFACE 的值
# install(CODE "
#     message(STATUS \"=== INSTALL PHASE ===\")
#     message(STATUS \"INTERFACE_INCLUDE = $<TARGET_PROPERTY:Algo,INTERFACE_INCLUDE_DIRECTORIES>\")
# ")
#[=========================[note: install(CODE ...) 的方式，无法正确输出 生成表达式的结果]=========================]


#[[
@brief: 使用 $<BUILD_INTERFACE:...> $<INSTALL_INTERFACE:...> 来只在对应阶段赋予相应的属性值； &&
        并在对应阶段，输出对应属性的值，比如 INTERFACE_INCLUDE_DIRECTORIES
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

+ output: -- subject1
+ output in Output Window：

    === BUILD PHASE ===
    INCLUDE = D:/Codes/zljgit/kerneltravel/build_tools/cmake/Part_II_Fundamentals/Chapter_16_Compiler_And_Linker_Essentials/16_3_Target_Property_Contexts/Installing/src/BUILD_ONLY;D:/Codes/zljgit/kerneltravel/build_tools/cmake/Part_II_Fundamentals/Chapter_16_Compiler_And_Linker_Essentials/16_3_Target_Property_Contexts/Installing/src/include

+ /output/lib/cmake/Algo/AlgoTargets.cmake 的片段：

    if(NOT CMAKE_VERSION VERSION_LESS "3.23.0")
    target_sources(Algo
        INTERFACE
        FILE_SET "algo_pub"
        TYPE "HEADERS"
        BASE_DIRS "${_IMPORT_PREFIX}/include"
        FILES "${_IMPORT_PREFIX}/include/Algo.h"
    )

    else()
    set_property(TARGET Algo
        APPEND PROPERTY INTERFACE_INCLUDE_DIRECTORIES
        "${_IMPORT_PREFIX}/include"
    )
    endif()

else() 分支中，对于我们设置的两个生成表达式，这里只用到了 INSTALL_INTERFACE 中的值，
BUILD_INTERFACE 的值没有被用到，说明 生成表达式 $<BUILD_INTERFACE> $<INSTALL_INTERFACE> 上下文确实起到作用了；

+ 不能用 install(CODE ...) 的方式，来输出安装阶段 Algo 的 INTERFACE_INCLUDE_DIRECTORIES，它不会对生成表达式正确求值，
    从现象上看，没有进行判断，将每个生成表达式的值都留下来了；

+ phenomenon:
    add_custom_command(TARGET Algo POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E echo "=== BUILD PHASE ==="
        COMMAND ${CMAKE_COMMAND} -E echo "INCLUDE = $<TARGET_PROPERTY:Algo,INTERFACE_INCLUDE_DIRECTORIES>"
    )
    如果 build 阶段，输出的是 INCLUDE_DIRECTORIES，output 窗口的输出，你会看到
    有重复的 xxx/src/include，因为有不同的地方，对这个属性值有影响：
        1. 下面这里就传递了一个（原来以为添加了两个，看输出总共只有两个，那这里应该就只算了一个，进行了去重可能）
            target_link_libraries(Algo
                PRIVATE
                    AlgoImpl
                    Compute
            )
        2. 下面这里传递一个
            target_sources(Algo PUBLIC FILE_SET algo_pub
                TYPE HEADERS
                BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/src/include
                FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/include/Algo.h
            )
        === BUILD PHASE ===
        INCLUDE = D:/Codes/zljgit/kerneltravel/build_tools/cmake/Part_II_Fundamentals/Chapter_16_Compiler_And_Linker_Essentials/16_3_Target_Property_Contexts/Installing/src/BUILD_ONLY;D:/Codes/zljgit/kerneltravel/build_tools/cmake/Part_II_Fundamentals/Chapter_16_Compiler_And_Linker_Essentials/16_3_Target_Property_Contexts/Installing/src/include;D:/Codes/zljgit/kerneltravel/build_tools/cmake/Part_II_Fundamentals/Chapter_16_Compiler_And_Linker_Essentials/16_3_Target_Property_Contexts/Installing/src/include
]]