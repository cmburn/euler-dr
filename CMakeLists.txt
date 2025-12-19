cmake_minimum_required(VERSION 3.31)
project(euler)

set(CMAKE_CXX_STANDARD 23)

option(EULER_DRAGONRUBY_BUILD "Build Dragonruby bindings" ON)

include(ExternalProject)

add_subdirectory(src)

add_library(dragonruby INTERFACE)

set(ARMADILLO_EXTERN_NAME armadillo-extern)
set(ARMADILLO_GIT_TAG 15.2.2)

ExternalProject_Add(${ARMADILLO_EXTERN_NAME}
        PREFIX ${PROJECT_BINARY_DIR}/${ARMADILLO_EXTERN_NAME}
        GIT_REPOSITORY https://gitlab.com/conradsnicta/armadillo-code.git
        GIT_TAG ${ARMADILLO_GIT_TAG}
        UPDATE_COMMAND ""
        CMAKE_ARGS
        -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>
        -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
        -DBUILD_SHARED_LIBS=OFF
        BUILD_BYPRODUCTS <INSTALL_DIR>/lib/libarmadillo.a
)

ExternalProject_Get_Property(${ARMADILLO_EXTERN_NAME} INSTALL_DIR)
file(MAKE_DIRECTORY ${INSTALL_DIR}/include)

add_library(armadillo STATIC IMPORTED GLOBAL)
set_target_properties(armadillo PROPERTIES
        IMPORTED_LOCATION "${INSTALL_DIR}/lib/libarmadillo.a"
        INTERFACE_INCLUDE_DIRECTORIES "${INSTALL_DIR}/include"
)

add_dependencies(armadillo ${ARMADILLO_EXTERN_NAME})

target_include_directories(dragonruby INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/dragonruby/include
)

#set(DRAGONRUBY_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/dragonruby/include)
#message(STATUS "DragonRuby Include Dir: ${DRAGONRUBY_INCLUDE_DIR}")