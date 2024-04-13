cmake_minimum_required(VERSION 3.20)
project(app LANGUAGES CXX C)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB SRCS "src/*.cpp" "src/*.h")

set(CORE_LIB_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/core/include")

add_executable(app "${SRCS}")

target_include_directories(app PRIVATE ${CORE_LIB_INCLUDE_DIR})
target_link_libraries(app core)
