
cmake_minimum_required(VERSION 3.20)
project(core LANGUAGES CXX C)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


file(GLOB SRCS "src/*.cpp" "include/*.h")

add_library("${CORE_LIB_NAME}" "${SRCS}")
target_include_directories(core PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include/core")

find_package(Vulkan REQUIRED)
target_link_libraries("${CORE_LIB_NAME}" PUBLIC "${Vulkan_LIBRARIES}")
