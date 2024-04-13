cmake_minimum_required(VERSION 3.20)
project(renderer LANGUAGES CXX C)

add_compile_options(-Wall -pedantic -Wextra)

set(CORE_LIB_NAME "core")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(app)
add_subdirectory(core)
