cmake_minimum_required(VERSION 3.16)
project(CrazeEmulator)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS "-g -Wall -Werror -Wuninitialized")

file(GLOB SOURCES "*.cpp")

add_executable(CrazeEmulator ${SOURCES})

if(UNIX)
	add_custom_target(clang-format
	COMMAND clang-format -i ${SOURCES}
	COMMENT "Running clang-format"
	)
add_dependencies(CrazeEmulator clang-format)
endif()



