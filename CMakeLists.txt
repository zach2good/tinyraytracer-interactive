cmake_minimum_required(VERSION 3.13)

project(TinyRaytracer)
enable_language(CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
message(STATUS "CMAKE_CXX_COMPILER_ID: ${CMAKE_CXX_COMPILER_ID}")

set(PROFILING ON)

# External Deps
add_subdirectory(ext)

# Profiling
add_library(Trace src/trace.cpp)

# TinyRaytracer
add_executable(TinyRaytracer src/tinyraytracer.cpp)
target_include_directories(TinyRaytracer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_compile_options(TinyRaytracer PUBLIC -O3)
target_link_libraries(TinyRaytracer PUBLIC glad SDL2-static imgui)

if(PROFILING)
    target_compile_options(TinyRaytracer PUBLIC -finstrument-functions-after-inlining -O0)
    target_link_libraries(TinyRaytracer PUBLIC Trace)
endif()