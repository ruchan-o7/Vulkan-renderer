
cmake_minimum_required(VERSION 3.20)
project(core LANGUAGES CXX C)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB SRCS 
	"src/*.cpp" 
	"src/renderer/*.cpp"
)

add_library("${CORE_LIB_NAME}" "${SRCS}")

target_include_directories(core PRIVATE
	"${CMAKE_CURRENT_SOURCE_DIR}/include/core"
	"${CMAKE_SOURCE_DIR}/external/spdlog/include"
	"${CMAKE_SOURCE_DIR}/external/spdlog/include"
	"${Vulkan_INCLUDE_DIRS}")

target_link_libraries("${CORE_LIB_NAME}" PUBLIC "${Vulkan_LIBRARIES}")
target_link_libraries("${CORE_LIB_NAME}" PUBLIC glm)
target_link_libraries("${CORE_LIB_NAME}" PUBLIC glfw)
target_precompile_headers(${CORE_LIB_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/pch.h")

target_compile_definitions(${CORE_LIB_NAME} PRIVATE $<$<PLATFORM_ID:Windows>:VR_WINDOWS>)

target_compile_definitions(${CORE_LIB_NAME} PRIVATE $<$<PLATFORM_ID:Linux>:VR_LINUX>)

target_compile_definitions(${CORE_LIB_NAME} PRIVATE $<$<PLATFORM_ID:Darwin>:VR_MACOS>)

target_compile_definitions(${CORE_LIB_NAME} PRIVATE  $<$<CONFIG:Debug>:VR_DEBUG>)

target_compile_definitions(${CORE_LIB_NAME} PRIVATE $<$<CONFIG:Release>:VR_RELEASE>)
