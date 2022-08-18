cmake_minimum_required(VERSION 3.22)
project(KarafutoLayerRaylibExample)

set(CMAKE_CXX_STANDARD 20)

find_package(raylib 4.2.0 QUIET)
if (NOT raylib_FOUND)
    include(FetchContent)
    FetchContent_Declare(
            raylib
            URL https://github.com/raysan5/raylib/archive/refs/tags/4.2.0.tar.gz
    )
    FetchContent_GetProperties(raylib)
    if (NOT raylib_POPULATED)
        set(FETCHCONTENT_QUIET NO)
        FetchContent_Populate(raylib)
        set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
        add_subdirectory(${raylib_SOURCE_DIR} ${raylib_BINARY_DIR} EXCLUDE_FROM_ALL)
    endif ()
endif ()

add_subdirectory(third-party/karafuto-core)
include_directories(${karafuto_core_SOURCE_DIR}/src)

if (MSVC)
    add_compile_definitions(_USE_MATH_DEFINES)
endif ()

file(GLOB SOURCES "**/*.hpp" "**/*.cpp" "*.cpp")

add_executable(KarafutoCoreRaylibExample ${SOURCES})
target_link_libraries(KarafutoCoreRaylibExample raylib libkcore glm)
