
set(var_with_internal def CACHE INTERNAL "test variable INTERNAL")

message("=== ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt")
message("after interval set")
message("var_with_internal = ${var_with_internal}")

message("verified: set cache variable 的方式确实是类似 FORCE")

message("")

#[[
@steps:
    例子的 top 目录：
    1. 先执行了 ./clean.sh
    2. 再执行的：cmake -B build -G "MinGW Makefiles" -Dvar_without_force="hg" -Dvar_with_interval="abc"

@output:
    === D:/Codes/zljgit/kerneltravel/build_tools/cmake/Chapter2_Fundamentals/6_3_Cache_Variables/6_3_1_Getting_And_Setting_Cache_Variables/CMakeLists.txt
    before interval set
    var_with_interval = abc

    ...

    === D:/Codes/zljgit/kerneltravel/build_tools/cmake/Chapter2_Fundamentals/6_3_Cache_Variables/6_3_1_Getting_And_Setting_Cache_Variables/attempts/set_cache_with_internal/CMakeLists.txt
    after interval set
    var_with_internal = def

    -- Configuring done (50.2s)

@summary:
    证明了 INTERNAL 的方式，确实是类似 FORCE 

]]