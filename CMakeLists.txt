cmake_minimum_required(VERSION 3.23 FATAL_ERROR)
project(euler)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_C_STANDARD 17)

option(EULER_DRAGONRUBY_BUILD "Build Dragonruby bindings" OFF)
option(EULER_NATIVE_BUILD "Build native bindings" OFF)
option(EULER_MATH_BUILD "Build Euler::Math module" OFF)
option(EULER_PHYSICS_BUILD "Build Euler::Physics module" ON)
option(EULER_DRAGONRUBY_PATH "Path to DragonRuby installation" "")
option(EULER_GUI_BUILD "Build Euler::GUI module" ON)
option(EULER_NET_BUILD "Build Euler::Net module" ON)
option(EULER_GRAPHICS_BUILD "Build Euler::Graphics module" ON)
option(EULER_EVENT_BUILD "Build Euler::Event module" ON)
option(BUILD_TESTS "Build tests" OFF)

include(ExternalProject)
include(FetchContent)

find_package(Ruby REQUIRED)

if (NOT EULER_DRAGONRUBY_BUILD AND NOT EULER_NATIVE_BUILD)
    # If EULER_DRAGONRUBY_PATH is set and EULER_NATIVE_BUILD is not set,
    # default to EULER_DRAGONRUBY_BUILD. Otherwise, default to EULER_NATIVE_BUILD.
    if (EULER_DRAGONRUBY_PATH)
        message(STATUS "EULER_DRAGONRUBY_PATH is set, defaulting to EULER_DRAGONRUBY_BUILD")
        set(EULER_DRAGONRUBY_BUILD ON)
    elseif (NOT EULER_DRAGONRUBY_BUILD)
        message(STATUS "EULER_DRAGONRUBY_PATH is not set, defaulting to EULER_NATIVE_BUILD")
        set(EULER_NATIVE_BUILD ON)
    endif ()
endif ()

if (EULER_NATIVE_BUILD)
    include(cmake/fetch_mruby.cmake)
endif ()

add_subdirectory(src)

set(BUILD_SHARED_LIBS OFF)
add_compile_definitions(EULER_GV_STATE="${EULER_GV_STATE}")

if (EULER_DRAGONRUBY_BUILD AND EULER_NATIVE_BUILD)
    message(FATAL_ERROR "EULER_DRAGONRUBY_BUILD and EULER_NATIVE_BUILD are mutually exclusive. Please enable only one of them.")
elseif (NOT EULER_DRAGONRUBY_BUILD AND NOT EULER_NATIVE_BUILD)
    message(FATAL_ERROR "Either EULER_DRAGONRUBY_BUILD or EULER_NATIVE_BUILD must be enabled. Please enable one of them.")
endif ()

if (EULER_DRAGONRUBY_BUILD)
    message(STATUS "Building DragonRuby runtime bindings")
    add_compile_definitions(EULER_DRAGONRUBY)
elseif (EULER_NATIVE_BUILD)
    message(STATUS "Building native runtime bindings")
    add_compile_definitions(EULER_NATIVE)
endif ()

# all versions require armadillo
set(ARMADILLO_GIT_TAG 15.2.2)
FetchContent_Declare(armadillo
        GIT_REPOSITORY https://gitlab.com/conradsnicta/armadillo-code.git
        GIT_TAG ${ARMADILLO_GIT_TAG}
        GIT_SHALLOW TRUE
)
option(STATIC_LIB "Build static libraries" ON)
if (NOT STATIC_LIB)
    message(FATAL_ERROR "Euler requires STATIC_LIB to be ON")
endif ()
FetchContent_MakeAvailable(armadillo)

#target_compile_definitions(armadillo PUBLIC
#        -DARMA_MAT_PREALLOC=2
#)

if (EULER_PHYSICS_BUILD)
    set(BOX2D_GIT_TAG c05c48738fbe5c27625e36c5f0cfbdaddfc8359a)
    FetchContent_Declare(box2d
            GIT_REPOSITORY https://github.com/erincatto/box2d
            GIT_TAG ${BOX2D_GIT_TAG}
            GIT_SHALLOW TRUE
    )
    FetchContent_MakeAvailable(box2d)
endif ()

if (EULER_NET_BUILD)
    set(PROTOBUF_GIT_TAG v34.0)
    FetchContent_Declare(protobuf
            GIT_REPOSITORY https://github.com/protocolbuffers/protobuf
            GIT_TAG ${PROTOBUF_GIT_TAG}
            GIT_SHALLOW TRUE
    )
    FetchContent_MakeAvailable(protobuf)
    set(GNS_TAG v1.4.1)
    FetchContent_Declare(gns
            GIT_REPOSITORY https://github.com/ValveSoftware/GameNetworkingSockets
            GIT_TAG ${GNS_TAG}
            GIT_SHALLOW TRUE
    )
    FetchContent_MakeAvailable(gns)
endif ()

if (EULER_DRAGONRUBY_BUILD)
    # TODO: Nuklear support
    if (NOT EULER_DRAGONRUBY_PATH)
        message(FATAL_ERROR "EULER_DRAGONRUBY_PATH is not set. Please set it to the path of your DragonRuby installation.")
    endif ()
    message(STATUS "Using DragonRuby installation at: ${EULER_DRAGONRUBY_PATH}")
    add_library(dragonruby INTERFACE)
    target_include_directories(dragonruby INTERFACE
            ${EULER_DRAGONRUBY_PATH}/include
    )
    set(MRUBY_VERSION 3.0.0)
elseif (EULER_NATIVE_BUILD)
    # if vulkan_sdk env var exists, use it instead of system headers
    if (DEFINED ENV{VULKAN_SDK})
        set(Vulkan_INCLUDE_DIR "$ENV{VULKAN_SDK}/include")
        include_directories(BEFORE SYSTEM $ENV{VULKAN_SDK}/include)
    endif ()

    find_package(Vulkan REQUIRED)

    # Native we pull in mruby, physfs, SDL, and Vulkan2D.

    # TODO: stabilize on 3.5.0 once it's released
    fetch_mruby(mruby
            GIT_REPOSITORY https://github.com/mruby/mruby
            GIT_TAG master
    )

    set(SPDLOG_GIT_COMMIT 79524ddd08a4ec981b7fea76afd08ee05f83755d)
    set(SPDLOG_USE_STD_FORMAT ON)
    FetchContent_Declare(spdlog
            GIT_REPOSITORY https://github.com/gabime/spdlog
            GIT_COMMIT ${SPDLOG_GIT_COMMIT}
            GIT_TAG v1.x
            GIT_SHALLOW TRUE
    )
    FetchContent_MakeAvailable(spdlog)

    set(PHYSFS_GIT_COMMIT d70c3fcf06814f8608c8327d3e8136063ee0133d)
    FetchContent_Declare(physfs
            GIT_REPOSITORY https://github.com/icculus/physfs/
            GIT_COMMIT ${PHYSFS_GIT_COMMIT}
            GIT_TAG main
            GIT_SHALLOW TRUE
    )
    FetchContent_MakeAvailable(physfs)

    set(SDL_GIT_TAG prerelease-3.3.6)
    set(SDL_X11_XTEST OFF)
    FetchContent_Declare(SDL3
            GIT_REPOSITORY https://github.com/libsdl-org/SDL/
            GIT_TAG ${SDL_GIT_TAG}
            GIT_SHALLOW TRUE
    )
    FetchContent_MakeAvailable(SDL3)

    set(VK2D_BUILD_SDL OFF)
    set(VULKAN2D_GIT_TAG 533c75e1c5991411f8779b375beeefc522d3d18a)
    FetchContent_Declare(Vulkan2D
            GIT_REPOSITORY https://github.com/PaoloMazzon/Vulkan2D/
            GIT_TAG ${VULKAN2D_GIT_TAG}
            GIT_SHALLOW FALSE
    )
    FetchContent_MakeAvailable(Vulkan2D)
    add_subdirectory(bin)
endif ()

if (BUILD_TESTS)
    if (NOT EULER_NATIVE_BUILD)
        message(FATAL_ERROR "Tests can only be built with EULER_NATIVE_BUILD enabled.")
    endif ()
    enable_testing()
    add_subdirectory(test)
    set(GTEST_GIT_TAG v1.17.0)
    FetchContent_Declare(googletest
            GIT_REPOSITORY https://github.com/google/googletest/
            GIT_TAG ${GTEST_GIT_TAG}
            GIT_SHALLOW TRUE
    )
    FetchContent_MakeAvailable(googletest)
endif ()

message(STATUS "Euler configuration:")
if (EULER_DRAGONRUBY_BUILD)
    message(STATUS "  Build type: DragonRuby")
else ()
    message(STATUS "  Build type: Native")
endif ()
message(STATUS "  Build Euler::Physics: ${EULER_PHYSICS_BUILD}")
message(STATUS "  Build Euler::Math: ${EULER_MATH_BUILD}")
message(STATUS "  Build Euler::Gui: ${EULER_GUI_BUILD}")
message(STATUS "  Build tests: ${BUILD_TESTS}")