cmake_minimum_required(VERSION 3.13)

project(TinyRaytracer)
enable_language(CXX)
set(CMAKE_CXX_STANDARD 14)

add_subdirectory(ext)

add_executable(TinyRaytracer
        src/tinyraytracer.cpp)

target_include_directories(TinyRaytracer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

target_compile_options(TinyRaytracer PUBLIC -O3)

target_link_libraries(TinyRaytracer PUBLIC glad SDL2-static imgui)