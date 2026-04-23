cmake_minimum_required(VERSION 3.23 FATAL_ERROR)
project(euler)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_C_STANDARD 17)

option(EULER_DRAGONRUBY_PATH "Path to DragonRuby installation" "")
option(EULER_GUI_BUILD "Build Euler::GUI module" OFF)
option(EULER_MATH_BUILD "Build Euler::Math module" ON)
option(EULER_NET_BUILD "Build Euler::Net module" OFF)
option(EULER_PHYSICS_BUILD "Build Euler::Physics module" ON)
option(EULER_WORLD_BUILD "Build Euler::World module" OFF)
option(EULER_GRAPHICS_BUILD "Build Euler::Graphics module" ON)

include(ExternalProject)
include(FetchContent)

find_package(Ruby REQUIRED)

# There was a semi-functional native backend at one point, so there remains
# some references to multiple backends in the codebase. I'd like to add one
# again at some point, but that's not a priority.
if (NOT EULER_DRAGONRUBY_PATH)
    message(FATAL_ERROR "EULER_DRAGONRUBY_PATH must be set to the path of your DragonRuby installation.")
endif ()
set(EULER_DRAGONRUBY_BUILD ON)

add_subdirectory(src)

set(BUILD_SHARED_LIBS OFF)
add_compile_definitions(EULER_GV_STATE="${EULER_GV_STATE}")

# TODO: attach to protobuf specifically instead
# protobuf has a tremendous amount of deprecated calls, so blame Google
add_compile_options(-Wno-deprecated-declarations -Wno-deprecated)

if (EULER_DRAGONRUBY_BUILD)
    message(STATUS "Building DragonRuby runtime bindings")
    add_compile_definitions(EULER_DRAGONRUBY)
endif ()

option(STATIC_LIB "Build static libraries" ON)
if (NOT STATIC_LIB)
    message(FATAL_ERROR "Euler requires STATIC_LIB to be ON")
endif ()

set(EIGEN_GIT_TAG 5.0.1)
FetchContent_Declare(eigen
        GIT_REPOSITORY https://gitlab.com/libeigen/eigen.git
        GIT_TAG ${EIGEN_GIT_TAG}
        GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(eigen)

if (EULER_PHYSICS_BUILD)
    set(BOX2D_GIT_TAG 5778d748bb06fdabcd46cb0d56646d8b2d6a4be7)
    FetchContent_Declare(box2d
            GIT_REPOSITORY https://github.com/erincatto/box2d
            GIT_TAG ${BOX2D_GIT_TAG}
            GIT_SHALLOW TRUE
    )
    FetchContent_MakeAvailable(box2d)
endif ()

if (EULER_GUI_BUILD)
    set(EULER_GRAPHICS_BUILD ON)
    set(NUKLEAR_GIT_TAG 4.12.8)
    FetchContent_Declare(nuklear
            GIT_REPOSITORY https://github.com/Immediate-Mode-UI/Nuklear/
            GIT_TAG ${NUKLEAR_GIT_TAG}
            GIT_SHALLOW TRUE
    )
    FetchContent_MakeAvailable(nuklear)
    add_library(nuklear INTERFACE)
    target_include_directories(nuklear INTERFACE
            ${nuklear_SOURCE_DIR}
    )
endif ()

set(PROTOBUF_GIT_TAG v34.0)
FetchContent_Declare(protobuf
        GIT_REPOSITORY https://github.com/protocolbuffers/protobuf
        GIT_TAG ${PROTOBUF_GIT_TAG}
        GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(protobuf)

if (EULER_NET_BUILD)
    # required by GNS
    set(GNS_TAG v1.4.1)
    FetchContent_Declare(gns
            GIT_REPOSITORY https://github.com/ValveSoftware/GameNetworkingSockets
            GIT_TAG ${GNS_TAG}
            GIT_SHALLOW TRUE
    )
    FetchContent_MakeAvailable(gns)
endif ()

if (EULER_WORLD_BUILD)
    add_compile_definitions(EULER_WORLD)
    set(NLOHMANN_GIT_TAG v3.12.0)
    FetchContent_Declare(nlohmann_json
            GIT_REPOSITORY https://github.com/nlohmann/json
            GIT_TAG ${NLOHMANN_GIT_TAG}
            GIT_SHALLOW TRUE
    )
    FetchContent_MakeAvailable(nlohmann_json)
endif ()

if (EULER_DRAGONRUBY_BUILD)
    if (NOT EULER_DRAGONRUBY_PATH)
        message(FATAL_ERROR "EULER_DRAGONRUBY_PATH is not set. Please set it to the path of your DragonRuby installation.")
    endif ()
    message(STATUS "Using DragonRuby installation at: ${EULER_DRAGONRUBY_PATH}")
    add_library(dragonruby INTERFACE)
    target_include_directories(dragonruby INTERFACE
            ${EULER_DRAGONRUBY_PATH}/include
    )
    set(MRUBY_VERSION 3.0.0)
    set(EARCUT_GIT_TAG f36ced7e50254738c4e5af1a239f5fb7b1094007)
    FetchContent_Declare(earcut
            GIT_REPOSITORY https://github.com/mapbox/earcut.hpp
            GIT_TAG ${EARCUT_GIT_TAG}
            GIT_SHALLOW TRUE
    )
    FetchContent_MakeAvailable(earcut)
endif ()

message(STATUS "Euler configuration:")
if (EULER_DRAGONRUBY_BUILD)
    message(STATUS "  Build type: DragonRuby")
endif ()
message(STATUS "  Build Euler::Graphics: ${EULER_GRAPHICS_BUILD}")
message(STATUS "  Build Euler::Gui: ${EULER_GUI_BUILD}")
message(STATUS "  Build Euler::Math: ${EULER_MATH_BUILD}")
message(STATUS "  Build Euler::Net: ${EULER_NET_BUILD}")
message(STATUS "  Build Euler::Physics: ${EULER_PHYSICS_BUILD}")
message(STATUS "  Build Euler::World: ${EULER_WORLD_BUILD}")
