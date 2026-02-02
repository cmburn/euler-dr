cmake_minimum_required(VERSION 3.23 FATAL_ERROR)
project(euler)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_C_STANDARD 17)

option(EULER_DRAGONRUBY_BUILD "Build Dragonruby bindings" OFF)
option(EULER_NATIVE_BUILD "Build native bindings" OFF)
option(EULER_DRAGONRUBY_PATH "Path to DragonRuby installation" "")
option(EULER_GUI_BUILD "Build Euler::GUI module" OFF)
option(EULER_MATH_BUILD "Build Euler::Math module" ON)
option(EULER_PHYSICS_BUILD "Build Euler::Physics module" ON)
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

if (EULER_NET_BUILD)
    set(GNS_COMMIT 517fff0cf6866ba163f4f016b0ef28f365c06c05)
    FetchContent_Declare(GameNetworkingSockets
            GIT_REPOSITORY https://github.com/ValveSoftware/GameNetworkingSockets
            GIT_COMMIT ${GNS_COMMIT}
            GIT_SHALLOW TRUE
    )
endif ()

if (EULER_FILE_BUILD)
    set(GLAZE_GIT_TAG v6.4.0)
    FetchContent_Declare(glaze
            GIT_REPOSITORY https://github.com/stephenberry/glaze
            GIT_TAG ${GLAZE_GIT_TAG}
            GIT_SHALLOW TRUE
    )
    FetchContent_MakeAvailable(glaze)
endif ()

if (EULER_MATH_BUILD)
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
endif ()

if (EULER_PHYSICS_BUILD)
    set(BOX2D_GIT_TAG c05c48738fbe5c27625e36c5f0cfbdaddfc8359a)
    FetchContent_Declare(box2d
            GIT_REPOSITORY https://github.com/erincatto/box2d
            GIT_TAG ${BOX2D_GIT_TAG}
            GIT_SHALLOW TRUE
    )
    FetchContent_MakeAvailable(box2d)
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

    # Note that the 3.5.0 is not yet out, however the native bindings for Euler
    # follow the repository HEAD and 3.5.0 is the next planned release. There
    # are certain API changes that Euler depends on that are not in 3.4.0.
    # We'll pin it at the 3.5 minor release and then track stable releases from
    # there.
    set(MRUBY_VERSION 3.5.0)
    set(MRUBY_EXTERN_NAME mruby)
    set(MRUBY_GIT_TAG 2da01c607f6341a18dcd726b46f3e7a4a2c49211)
    FetchContent_Declare(mruby
            GIT_REPOSITORY https://github.com/mruby/mruby
            GIT_COMMIT ${MRUBY_GIT_TAG}
            GIT_SHALLOW TRUE
    )
    FetchContent_MakeAvailable(mruby)

    set(PHYSFS_GIT_TAG release-3.2.0)
    FetchContent_Declare(physfs
            GIT_REPOSITORY https://github.com/icculus/physfs/
            GIT_TAG ${PHYSFS_GIT_TAG}
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
message(STATUS "  Build Euler::Net: ${EULER_NET_BUILD}")
message(STATUS "  Build Euler::Gui: ${EULER_GUI_BUILD}")
message(STATUS "  Build Euler::File: ${EULER_FILE_BUILD}")
message(STATUS "  Build tests: ${BUILD_TESTS}")