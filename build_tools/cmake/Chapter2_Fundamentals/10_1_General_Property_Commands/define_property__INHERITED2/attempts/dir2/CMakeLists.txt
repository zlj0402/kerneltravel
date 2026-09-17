add_library(Utils2 SHARED
	../../src/utils.cpp
)

message("=== ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt")

get_property(result TARGET Utils2 PROPERTY MY_THING)
message("Utils2 MY_THING result = ${result}")
unset(result)