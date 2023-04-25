cmake_minimum_required(VERSION 3.16)
project(CPP8080)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS "-g -Wall -Werror -Wuninitialized")

file(GLOB SOURCES "*.cpp" "cpu/*.cpp" "disassemble/*.cpp" "error/*.cpp")

add_executable(CPP8080 ${SOURCES})

if(UNIX)
	file(GLOB HEADER_SOURCES "*.cpp" "cpu/*.cpp" "disassemble/*.cpp" "error/*.cpp" "disassemble/*.hpp" "cpu/*.hpp" "error/*.hpp")
	add_custom_target(clang-format
	COMMAND clang-format -i ${HEADER_SOURCES}
	COMMENT "Running clang-format"
	)
add_dependencies(CPP8080 clang-format)
endif()



