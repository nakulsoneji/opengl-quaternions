cmake_minimum_required(VERSION 3.5)

project(
  OpenGlTest
  VERSION 1.0
  LANGUAGES C CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

file(CREATE_LINK "${CMAKE_BINARY_DIR}/compile_commands.json"
  "${CMAKE_SOURCE_DIR}/compile_commands.json" SYMBOLIC)

add_executable(app
  "src/app.cpp"
  "src/glad.c"
  "src/shader.cpp"
  "src/stb_image.c")

target_include_directories(app
  PRIVATE "include/")

target_link_libraries(app
  PRIVATE "GL"
  PRIVATE "glfw")


