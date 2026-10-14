cmake_minimum_required(VERSION 3.25)
project(DefinitionsScopeTest LANGUAGES CXX)

# 创建测试用的空源文件
file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/target1.cpp" "int t1() { return 0; }")
file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/target2.cpp" "int t2() { return 0; }")

message(STATUS "=== 测试 add_compile_options() 的作用域 ===")

# 第一个目标
add_library(Target1 ${CMAKE_CURRENT_BINARY_DIR}/target1.cpp)
get_target_property(T1_OPS Target1 COMPILE_OPTIONS)
message(STATUS "Target1 (定义前): ${T1_OPS}")

# 添加编译选项（影响后面定义的 target）
add_compile_options(-Wall -Wextra -DTEST_OPTION)

# 第二个目标
add_library(Target2 ${CMAKE_CURRENT_BINARY_DIR}/target2.cpp)
get_target_property(T2_OPS Target2 COMPILE_OPTIONS)
message(STATUS "Target2 (定义后): ${T2_OPS}")


#[===============[note: 尝试输出 target 的 COMPILE_OPTIONS 属性值]===============]

add_custom_command(TARGET Target1 POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E echo "=== Target1 OPTIONS ==="
    COMMAND ${CMAKE_COMMAND} -E echo "Target1 COMPILE_OPTIONS = $<TARGET_PROPERTY:Target1,COMPILE_OPTIONS>"
)

add_custom_command(TARGET Target2 POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E echo "=== Target2 OPTIONS ==="
    COMMAND ${CMAKE_COMMAND} -E echo "Target2 COMPILE_OPTIONS = $<TARGET_PROPERTY:Target2,COMPILE_OPTIONS>"
)
#[===============[note: 尝试输出 target 的 COMPILE_OPTIONS 属性值]===============]

#[[
@brief: COMPILE_OPTIONS 目录属性作用范围（当前 CMakeLists.txt）
@author: liangj.zhang
@date: 2026/10/14

@steps:
    或者直接从安排的顺序上说吧：
    1. target1 定义在两个 add_compile_options() 之前
    2. target2 定义在一个之后，在一个之前；

@output:

    [build] === Target1 OPTIONS ===
    [build] Target1 COMPILE_OPTIONS = 
    [build] [4/4 100% :: 1.083] Linking CXX static library Debug\libTarget2.a
    [build] === Target2 OPTIONS ===
    [build] Target2 COMPILE_OPTIONS = -Wall;-Wextra;-DTEST_OPTION

@summary:
    + add_custom_command 的方式，可以看到 COMPILE_OPTIONS 的结果：
        两个 target 最后的 COMPILE_OPTIONS 的属性值，Target1 没有， Target2 有；
        所以就像文档中说的：调用之前创建的任何目标不受影响。与其他目录属性命令相比，这种行为更接近开发者的直觉预期。

@ps:
    还可以通过 /build/compile_commands.json 当中 target2.cpp.obj 的 command 项中去查看，命令行中是否带上了 options，下面一行一样：
    "command": "D:\\Tools\\MSYS2\\ucrt64\\bin\\g++.exe -DCMAKE_INTDIR=\\\"RelWithDebInfo\\\" -O2 -g -DNDEBUG -Wall -Wextra -DTEST_OPTION -o CMakeFiles\\Target2.dir\\RelWithDebInfo\\target2.cpp.obj -c D:\\Codes\\zljgit\\kerneltravel\\build_tools\\cmake\\Part_II_Fundamentals\\Chapter_16_Compiler_And_Linker_Essentials\\16_4_Directory_Properties_And_Commands\\add_compile_options\\build\\target2.cpp",
]]