cmake_minimum_required(VERSION 3.25)
project(DefinitionsScopeTest LANGUAGES CXX)

# 创建测试用的空源文件
file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/target1.cpp" "int t1() { return 0; }")
file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/target2.cpp" "int t2() { return 0; }")

message(STATUS "=== 测试 add_compile_definitions() 的作用域 ===")

# 第一个目标
add_library(Target1 ${CMAKE_CURRENT_BINARY_DIR}/target1.cpp)
get_target_property(T1_DEFS Target1 COMPILE_DEFINITIONS)
message(STATUS "Target1 (定义前): ${T1_DEFS}")

# 添加编译定义
add_compile_definitions(FIRST_DEFINE=1)

# 第二个目标
add_library(Target2 ${CMAKE_CURRENT_BINARY_DIR}/target2.cpp)
get_target_property(T2_DEFS Target2 COMPILE_DEFINITIONS)
message(STATUS "Target2 (定义后): ${T2_DEFS}")

# 再次添加编译定义
add_compile_definitions(SECOND_DEFINE=2)


#[===============[note: 尝试输出 target 的 COMPILE_DEFINITIONS 属性值]===============]
include(CMakePrintHelpers)
cmake_print_properties(
    TARGETS Target1 Target2
    PROPERTIES COMPILE_DEFINITIONS
)

add_custom_command(TARGET Target1 POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E echo "=== Target1 DEFINITIONS ==="
    COMMAND ${CMAKE_COMMAND} -E echo "Target1 COMPILE_DEFINITIONS = $<TARGET_PROPERTY:Target1,COMPILE_DEFINITIONS>"
)

add_custom_command(TARGET Target2 POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E echo "=== Target2 DEFINITIONS ==="
    COMMAND ${CMAKE_COMMAND} -E echo "Target2 COMPILE_DEFINITIONS = $<TARGET_PROPERTY:Target2,COMPILE_DEFINITIONS>"
)
#[===============[note: 尝试输出 target 的 COMPILE_DEFINITIONS 属性值]===============]

#[[
@brief: COMPILE_DEFINITIONS 目录属性作用范围（当前 CMakeLists.txt）
@author: liangj.zhang
@date: 2026/10/14

@steps:
    或者直接从安排的顺序上说吧：
    1. target1 定义在两个 add_compile_definitions() 之前
    2. target2 定义在一个之后，在一个之前；

@output:
    [cmake]  Properties for TARGET Target1:
    [cmake]    Target1.COMPILE_DEFINITIONS = <NOTFOUND>
    [cmake]  Properties for TARGET Target2:
    [cmake]    Target2.COMPILE_DEFINITIONS = <NOTFOUND>

    [build] === Target1 DEFINITIONS ===
    [build] Target1 COMPILE_DEFINITIONS = FIRST_DEFINE=1;SECOND_DEFINE=2
    [build] [4/4 100% :: 1.113] Linking CXX static library Debug\libTarget2.a
    [build] === Target2 DEFINITIONS ===
    [build] Target2 COMPILE_DEFINITIONS = FIRST_DEFINE=1;SECOND_DEFINE=2

@summary:
    + 第一次只用了 get_target_property() 的方式，得到的结果是 <NOTFOUND>
        + AI 的解释是：因为 add_compile_definitions() 设置的是目录属性 COMPILE_DEFINITIONS，而不是目标属性。定义在生成构建系统时被应用到所有目标，但不会存储在目标的 COMPILE_DEFINITIONS 属性中。
        于是我采取了，验证生成器表达式的方式，因为最后还是要赋值到 target 的身上；
    + 后面加了 add_custom_command 的方式，可以看到 COMPILE_DEFINITIONS 的结果：
        两个 target 最后的 COMPILE_DEFINITIONS 的属性值，都有 FIRST_DEFINE / SECOND_DEFINE
        所以就像文档中说的：add_compile_definitions() 仍然影响在同一目录作用域中创建的所有目标，无论这些目标是在 add_compile_definitions() 调用之前还是之后创建的。

@ps:
    还可以通过 /build/compile_commands.json 当中 target1.cpp.obj 的 command 项中去查看，命令行中是否带上了 definitions，下面一行一样：
    "command": "D:\\Tools\\MSYS2\\ucrt64\\bin\\g++.exe -DFIRST_DEFINE=1 -DSECOND_DEFINE=2 -DCMAKE_INTDIR=\\\"RelWithDebInfo\\\" -O2 -g -DNDEBUG -o CMakeFiles\\Target1.dir\\RelWithDebInfo\\target1.cpp.obj -c D:\\Codes\\zljgit\\kerneltravel\\build_tools\\cmake\\Part_II_Fundamentals\\Chapter_16_Compiler_And_Linker_Essentials\\16_4_Directory_Properties_And_Commands\\add_compile_definitions\\build\\target1.cpp",
]]