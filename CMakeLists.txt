cmake_minimum_required(VERSION 3.5)

project(opengl_test VERSION 1.0
                DESCRIPTION "opengl testing"
                LANGUAGES CXX C)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(CREATE_LINK
  "${CMAKE_BINARY_DIR}/compile_commands.json"
  "${CMAKE_SOURCE_DIR}/compile_commands.json" SYMBOLIC)

file(GLOB SRC_FILES CONFIGURE_DEPENDS
  "${CMAKE_SOURCE_DIR}/src/*.c"
  "${CMAKE_SOURCE_DIR}/src/*.cpp")

add_executable(app ${SRC_FILES})

target_include_directories(app
  PRIVATE "${CMAKE_SOURCE_DIR}/include")

target_link_libraries(app
  PRIVATE glfw)
