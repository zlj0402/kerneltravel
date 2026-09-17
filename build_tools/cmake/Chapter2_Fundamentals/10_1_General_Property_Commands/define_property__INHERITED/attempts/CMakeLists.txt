add_executable(mainTool2
	../src/main.cpp
)

message("=== ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt")

#[=========================[]=========================]
get_property(result3 TARGET mainTool2 PROPERTY MY_THING)
message("mainTool2 MY_THING result = ${result3}")
#[=========================[]=========================]
