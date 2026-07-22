cmake_minimum_required(VERSION 3.23 FATAL_ERROR)
project(euler)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_C_STANDARD 17)

option(EULER_DRAGONRUBY_PATH "Path to DragonRuby installation" "")
option(EULER_MATH_BUILD "Build Euler::Math module" ON)
option(EULER_NET_BUILD "Build Euler::Net module" ON)
option(EULER_PHYSICS_BUILD "Build Euler::Physics module" ON)
option(EULER_WORLD_BUILD "Build Euler::World module" ON)
option(EULER_GRAPHICS_BUILD "Build Euler::Graphics module" ON)
option(EULER_DRAGONRUBY_BUILD "Build as a DragonRuby extension" OFF)
option(EULER_NATIVE_BUILD "Build as a standalone engine" ON)

include(ExternalProject)
include(FetchContent)

find_package(Ruby REQUIRED)

if (NOT EULER_DRAGONRUBY_BUILD AND NOT EULER_NATIVE_BUILD)
    if (EULER_DRAGONRUBY_PATH)
        set(EULER_DRAGONRUBY_BUILD ON)
    else ()
        set(EULER_NATIVE_BUILD ON)
    endif ()
endif ()

if (EULER_DRAGONRUBY_BUILD)
    message(STATUS "Building DragonRuby runtime bindings")
    add_compile_definitions(EULER_DRAGONRUBY)
elseif (EULER_NATIVE_BUILD)
    add_compile_definitions(EULER_NATIVE)
endif ()

add_subdirectory(src)

set(BUILD_SHARED_LIBS OFF)
add_compile_definitions(EULER_GV_STATE="${EULER_GV_STATE}")


option(STATIC_LIB "Build static libraries" ON)
if (NOT STATIC_LIB)
    message(FATAL_ERROR "Euler requires STATIC_LIB to be ON")
endif ()

add_subdirectory(extern)

message(STATUS "Euler configuration:")
if (EULER_DRAGONRUBY_BUILD)
    message(STATUS "  Build type: DragonRuby")
elseif (EULER_NATIVE_BUILD)
    message(STATUS "  Build type: Native")
endif ()
message(STATUS "  Build Euler::Graphics: ${EULER_GRAPHICS_BUILD}")
message(STATUS "  Build Euler::Math: ${EULER_MATH_BUILD}")
message(STATUS "  Build Euler::Net: ${EULER_NET_BUILD}")
message(STATUS "  Build Euler::Physics: ${EULER_PHYSICS_BUILD}")
message(STATUS "  Build Euler::World: ${EULER_WORLD_BUILD}")
