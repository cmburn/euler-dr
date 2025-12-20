cmake_minimum_required(VERSION 3.23 FATAL_ERROR)
project(euler)

set(CMAKE_CXX_STANDARD 23)

option(EULER_DRAGONRUBY_BUILD "Build Dragonruby bindings" ON)
option(EULER_MATH_BUILD "Build Euler::Math module" ON)
option(EULER_PHYSICS_BUILD "Build Euler::Physics module" ON)
option(EULER_DRAGONRUBY_PATH "Path to DragonRuby installation" "")

include(ExternalProject)
include(FetchContent)

add_subdirectory(src)

if (EULER_DRAGONRUBY_BUILD)
    if (NOT EULER_DRAGONRUBY_PATH)
        message(FATAL_ERROR "EULER_DRAGONRUBY_PATH is not set. Please set it to the path of your DragonRuby installation.")
    endif ()
    add_library(dragonruby INTERFACE)
    #    target_include_directories(dragonruby INTERFACE
    #            ${CMAKE_CURRENT_SOURCE_DIR}/dragonruby/include
    #    )
    target_include_directories(dragonruby INTERFACE
            ${EULER_DRAGONRUBY_PATH}/include
    )
    set(MRUBY_VERSION 3.0.0)
else ()
    #message(FATAL_ERROR "Currently DragonRuby is the only supported runtime.")
    # Note that the 3.5.0 is not yet out, however the native bindings for Euler
    # follow the repository HEAD and 3.5.0 is the next planned release. There
    # are certain API changes that Euler depends on that are not in 3.4.0.
    set(MRUBY_VERSION 3.5.0)
    set(MRUBY_EXTERN_NAME mruby-extern)
    set(MRUBY_GIT_TAG 2da01c607f6341a18dcd726b46f3e7a4a2c49211)
endif ()

if (EULER_MATH_BUILD)
    #    set(ARMADILLO_EXTERN_NAME armadillo-extern)
    set(ARMADILLO_GIT_TAG 15.2.2)

    FetchContent_Declare(armadillo
            GIT_REPOSITORY https://gitlab.com/conradsnicta/armadillo-code.git
            GIT_TAG ${ARMADILLO_GIT_TAG}
            CMAKE_ARGS
            -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>
            -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
            -DBUILD_SHARED_LIBS=OFF
            -DSTATIC_LIB=ON
    )
    FetchContent_MakeAvailable(armadillo)

    #    FetchContent_Declare(${ARMADILLO_EXTERN_NAME}
    #            PREFIX ${PROJECT_BINARY_DIR}/${ARMADILLO_EXTERN_NAME}
    #            GIT_REPOSITORY https://gitlab.com/conradsnicta/armadillo-code.git
    #            GIT_TAG ${ARMADILLO_GIT_TAG}
    #            UPDATE_COMMAND ""
    #            CMAKE_ARGS
    #            -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>
    #            -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
    #            -DBUILD_SHARED_LIBS=OFF
    #            -DSTATIC_LIB=ON
    #            BUILD_BYPRODUCTS <INSTALL_DIR>/lib/libarmadillo.a
    #    )
    #
    #    FetchContent_MakeAvailable(${ARMADILLO_EXTERN_NAME})
    #
    #    FetchContent_GetProperties(${ARMADILLO_EXTERN_NAME})
    #    ExternalProject_Get_Property(${ARMADILLO_EXTERN_NAME} INSTALL_DIR)
    #    set(ARMADILLO_INSTALL_DIR ${ARMADILLO_EXTERN_NAME}_INSTALL_DIR)
    #    file(MAKE_DIRECTORY ${ARMADILLO_INSTALL_DIR}/include)
    #
    #    add_library(armadillo STATIC IMPORTED GLOBAL)
    #    set_target_properties(armadillo PROPERTIES
    #            IMPORTED_LOCATION "${INSTALL_DIR}/lib/libarmadillo.a"
    #            INTERFACE_INCLUDE_DIRECTORIES "${INSTALL_DIR}/include"
    #    )
    #
    #    add_dependencies(armadillo ${ARMADILLO_EXTERN_NAME})
endif ()

if (EULER_PHYSICS_BUILD)
    set(BOX2D_GIT_TAG c05c48738fbe5c27625e36c5f0cfbdaddfc8359a)
    FetchContent_Declare(box2d
            GIT_REPOSITORY https://github.com/erincatto/box2d
            GIT_TAG ${BOX2D_GIT_TAG}
            CMAKE_ARGS
            -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>
            -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
            -DBUILD_SHARED_LIBS=OFF
            -DSTATIC_LIB=ON
    )
    FetchContent_MakeAvailable(box2d)
endif ()

#if (EULER_PHYSICS_BUILD)
#    set(BOX2D_EXTERN_NAME box2d-extern)
#    set(BOX2D_GIT_TAG c05c48738fbe5c27625e36c5f0cfbdaddfc8359a)
#
#    ExternalProject_Add(${BOX2D_EXTERN_NAME}
#            PREFIX ${PROJECT_BINARY_DIR}/${BOX2D_EXTERN_NAME}
#            GIT_REPOSITORY https://github.com/erincatto/box2d
#            GIT_TAG ${BOX2D_GIT_TAG}
#            UPDATE_COMMAND ""
#            CMAKE_ARGS
#            -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>
#            -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
#            -DBUILD_SHARED_LIBS=OFF
#            -DSTATIC_LIB=ON
#            BUILD_BYPRODUCTS <INSTALL_DIR>/src/${BOX2D_EXTERN_NAME}-build/src/libbox2dd.a
#    )
#
#    ExternalProject_Get_Property(${BOX2D_EXTERN_NAME} INSTALL_DIR)
#    file(MAKE_DIRECTORY ${INSTALL_DIR}/include)
#    add_library(box2d STATIC IMPORTED GLOBAL)
#    set_target_properties(box2d PROPERTIES
#            IMPORTED_LOCATION "${INSTALL_DIR}/src/${BOX2D_EXTERN_NAME}-build/src/libbox2dd.a"
#            INTERFACE_INCLUDE_DIRECTORIES "${INSTALL_DIR}/include"
#    )
#endif ()
#
#set(GLAZE_EXTERN_NAME glaze-extern)
#set(GLAZE_GIT_TAG v6.4.0)
#ExternalProject_Add(${GLAZE_EXTERN_NAME}
#        PREFIX ${PROJECT_BINARY_DIR}/${GLAZE_EXTERN_NAME}
#        GIT_REPOSITORY https://github.com/stephenberry/glaze
#        GIT_TAG ${GLAZE_GIT_TAG}
#        UPDATE_COMMAND ""
#        CMAKE_ARGS
#        -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>
#        -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
#        -DBUILD_SHARED_LIBS=OFF
#)

#set(DRAGONRUBY_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/dragonruby/include)
#message(STATUS "DragonRuby Include Dir: ${DRAGONRUBY_INCLUDE_DIR}")