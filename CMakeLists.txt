cmake_minimum_required(VERSION 3.7)
project(testBgfx)

set(CORE_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(CORE_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

include_directories(${CORE_INCLUDE_DIR})

file(GLOB_RECURSE CORE_SOURCE_FILES ${CORE_SOURCE_DIR}/*.cpp ${CORE_SOURCE_DIR}/*.c ${CORE_SOURCE_DIR}/*.h ${CORE_SOURCE_DIR}/*.hpp)
add_library(core STATIC ${CORE_SOURCE_FILES})

# this is heuristically generated, and may not be correct
find_package(bgfx CONFIG REQUIRED)
# note: 1 additional targets are not displayed.
target_link_libraries(core PRIVATE bgfx::bx bgfx::bgfx bgfx::bimg bgfx::bimg_decode)

# this is heuristically generated, and may not be correct
find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(core PRIVATE glfw)

# Or use the header-only version
find_package(spdlog CONFIG REQUIRED)
target_link_libraries(core PRIVATE spdlog::spdlog_header_only)

add_subdirectory(demo)