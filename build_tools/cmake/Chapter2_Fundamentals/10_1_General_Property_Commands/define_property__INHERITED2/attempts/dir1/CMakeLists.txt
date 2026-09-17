add_executable(mainTool2
	../../src/main.cpp
)

message("=== ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt")

#[=======================[]=======================]
set_property(DIRECTORY PROPERTY MY_THING HiThere)
#[=======================[]=======================]

define_property(TARGET PROPERTY MY_THING INHERITED)

get_property(result TARGET mainTool2 PROPERTY MY_THING)
message("mainTool2 MY_THING result = ${result}")
unset(result)