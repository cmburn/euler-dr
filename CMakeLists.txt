cmake_minimum_required(VERSION 3.23 FATAL_ERROR)
project(euler)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_C_STANDARD 17)

option(EULER_DRAGONRUBY_PATH "Path to DragonRuby installation" "")
option(EULER_MATH_BUILD "Build Euler::Math module" ON)
option(EULER_NET_BUILD "Build Euler::Net module" OFF)
option(EULER_PHYSICS_BUILD "Build Euler::Physics module" ON)
option(EULER_WORLD_BUILD "Build Euler::World module" OFF)
option(EULER_GRAPHICS_BUILD "Build Euler::Graphics module" ON)
option(EULER_DRAGONRUBY_BUILD "Build as a DragonRuby extension" OFF)
option(EULER_NATIVE_BUILD "Build as a standalone engine" OFF)

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
        GIT_COMMIT ${EIGEN_GIT_TAG}
        GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(eigen)

if (EULER_PHYSICS_BUILD)
    set(BOX2D_GIT_TAG c05c48738fbe5c27625e36c5f0cfbdaddfc8359a)
    FetchContent_Declare(box2d
            GIT_REPOSITORY https://github.com/erincatto/box2d
            GIT_COMMIT ${BOX2D_GIT_TAG}
            GIT_SHALLOW TRUE
    )
    FetchContent_MakeAvailable(box2d)
endif ()

if (EULER_GRAPHICS_BUILD)
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
elseif (EULER_NATIVE_BUILD)
    # sdl3, Vulkan2D, physfs, mruby
    set(SDL_GIT_TAG release-3.4.12)
    FetchContent_Declare(SDL3
            GIT_REPOSITORY https://github.com/libsdl-org/SDL
            GIT_TAG ${SDL_GIT_TAG}
            GIT_SHALLOW TRUE
    )
    FetchContent_MakeAvailable(SDL3)

    set(VK2D_BUILD_SDL OFF)
    set(VK2D_GIT_TAG 4fe6b8f980755abd207822e187f53131979df2bc)
    FetchContent_Declare(VK2D
            GIT_REPOSITORY https://github.com/PaoloMazzon/Vulkan2D
            GIT_TAG ${VK2D_GIT_TAG}
            GIT_SHALLOW TRUE
    )
    FetchContent_MakeAvailable(VK2D)

    set(PHYSFS_GIT_TAG 23d0b5696445d86262221519c3db3ae351376da0)
    FetchContent_Declare(physfs
            GIT_REPOSITORY https://github.com/icculus/physfs
            GIT_TAG ${PHYSFS_GIT_TAG}
            GIT_SHALLOW TRUE
    )
endif ()

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
