message("${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt")

set(same_var "non cache variable")
set(same_var "global cache variable" CACHE STRING "")

message("same_var = ${same_var}")

message("")


#[[
@steps:
1. ./clean.sh
2. cmake -B build -G "MinGW Makefiles" -Dvar_without_force="hg" -Dvar_with_interval="abc"

@output:
D:/Codes/zljgit/kerneltravel/build_tools/cmake/Chapter2_Fundamentals/6_3_Cache_Variables/6_3_1_Getting_And_Setting_Cache_Variables/attempts/set_cache_variable_after_non_cache/CMakeLists.txt
same_var = non cache variab

@summary:
至少 3.25 版本的 CMake 上，第一次定义 cache 变量的时候，不会删除已经存在的非同名变量；
]]