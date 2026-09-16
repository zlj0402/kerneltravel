set(var_without_force def CACHE STRING "test variable without force")

message("=== ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt")
message("var_without_force = ${var_without_force}")

#[[
@steps:
    1. 先执行 cmake -B build -G "MinGW Makefiles" -Dvar_without_force="hg"

    @output:
    === D:/Codes/zljgit/kerneltravel/build_tools/cmake/Chapter2_Fundamentals/6_3_Cache_Variables/6_3_1_Getting_And_Setting_Cache_Variables/attempts/set_cache_without_force/CMakeLists.txt
    var_without_force = hg

    CMakeCache.txt 当中：
    //test variable without force
    var_without_force:STRING=hg

@note:
    运行上面命令的时候，这里的 CMakeLists.txt 就已经写上了 set(var_without_force def CACHE STRING "test variable without force")
    输出确实 hg，说明 if not-set, set
]]

#[=================================[侧面证明，set 语句在该文件中是有执行到的]=================================]
# set(var_without_force abc CACHE STRING "test variable without force" FORCE)
# message("var_without_force = ${var_without_force}")

message("verified: cache 变量是未设置时才定义（命令行中的定义高于其他方式，包括 CMakeCache.txt 当中）")


message("")