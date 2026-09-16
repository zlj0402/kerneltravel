
set(var_with_internal "local internal var")
message("var_with_internal local = ${var_with_internal}")
message("var_with_internal global = $CACHE{var_with_internal}")

MESSAGE("")

#[[
@steps:
1. ./clean.sh
2. cmake -B build -G "MinGW Makefiles" -Dvar_without_force="hg" -Dvar_with_interval="abc"

@output:
var_with_internal local = local internal var
var_with_internal global = def

@summary:
1. 证明 ${同名变量} 优先获取非 cache 变量；
2. $CACHE{变量} 可以获取全局变量值；
]]