cmake_minimum_required(VERSION 3.20)
project(renderer LANGUAGES CXX C)

add_definitions(-D_SILENCE_ALL_MS_EXT_DEPRECATION_WARNINGS)


find_package(Vulkan REQUIRED)

add_compile_options(-Wall -pedantic -Wextra -Wno-missing-field-initializers)
include_directories("core" "${Vulkan_INCLUDE_DIRS}")

set(CORE_LIB_NAME "core")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(external/glm)
add_subdirectory(external/glfw)

set(SPDLOG_BUILD_WARNINGS OFF)
add_subdirectory(external/spdlog)

add_subdirectory(app)
add_subdirectory(core)
