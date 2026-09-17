
#[====================[subject3: 看看自定义变量 get_property(...DEFINED/SET) 情况]====================]
message("=== ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt")
set(self_defined_var "not cache var")

get_property(is_defined VARIABLE PROPERTY self_defined_var DEFINED)
get_property(is_set VARIABLE PROPERTY self_defined_var SET)

message("get_property(...DEFINED) way to get CMAKE_SOURCE_DIR defined or not, result:
	${is_defined}"
)
message("get_property(...SET) way to get CMAKE_SOURCE_DIR set or not, result:
	${is_set}"
)
#[====================[subject3: 看看自定义变量 get_property(...DEFINED/SET) 情况]====================]
