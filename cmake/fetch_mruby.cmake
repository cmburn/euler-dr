macro(fetch_mruby TARGET)
    # We have to build mruby at configure time as it does not separate the
    # configure and build steps, so we need to use ExternalProject instead of
    # FetchContent.
    include (ExternalProject)
    set(options "")
    set(oneValueArgs
            # required
            GIT_REPOSITORY
            GIT_TAG
            # optional. This is the config.rb file to use. If not specified,
            # the default config from /config/mruby/config.rb will be used.
            CONFIG
    )
    set(multiValueArgs "")
    cmake_parse_arguments(fetch_mruby
            "${options}"
            "${oneValueArgs}"
            "${multiValueArgs}"
            ${ARGN}
    )
    if (NOT fetch_mruby_GIT_REPOSITORY)
        message(FATAL_ERROR "GIT_REPOSITORY is required for fetch_mruby")
    endif ()
    if (NOT fetch_mruby_GIT_TAG)
        message(FATAL_ERROR "GIT_TAG is required for fetch_mruby")
    endif ()

    if (NOT fetch_mruby_CONFIG)
        set(fetch_mruby_CONFIG ${CMAKE_SOURCE_DIR}/config/mruby/config.rb)
    endif ()
    message(STATUS "Fetching mruby from ${fetch_mruby_GIT_REPOSITORY} at tag ${fetch_mruby_GIT_TAG}")
    message(STATUS "Using mruby config: ${fetch_mruby_CONFIG}")

    set(MRUBY_SRC_DIR ${CMAKE_BINARY_DIR}/_deps/${TARGET}/src)
    set(MRUBY_BIN_DIR ${MRUBY_SRC_DIR}/build/host)

    ExternalProject_Add(${TARGET}-build
            GIT_REPOSITORY ${fetch_mruby_GIT_REPOSITORY}
            GIT_TAG ${fetch_mruby_GIT_TAG}
            GIT_SHALLOW TRUE
            SOURCE_DIR ${MRUBY_SRC_DIR}
            BINARY_DIR ${MRUBY_SRC_DIR}
            BUILD_COMMAND ${CMAKE_COMMAND} -E echo "No separate build step for mruby"
            CONFIGURE_COMMAND cd ${MRUBY_SRC_DIR} && ${CMAKE_COMMAND} -E env CC=${CMAKE_C_COMPILER} CXX=${CMAKE_CXX_COMPILER} MRUBY_CONFIG=${fetch_mruby_CONFIG} rake all
            INSTALL_COMMAND ""
            BUILD_BYPRODUCTS ${MRUBY_BIN_DIR}/lib/libmruby.a
    )

    add_library(${TARGET} INTERFACE)
    target_link_libraries(${TARGET} INTERFACE ${MRUBY_BIN_DIR}/lib/libmruby.a)
    target_include_directories(${TARGET} INTERFACE ${MRUBY_BIN_DIR}/include)
    target_compile_definitions(${TARGET} INTERFACE
            -DMRB_USE_CXX_EXCEPTION
            -DMRB_USE_CXX_ABI
            -DMRB_USE_SET
            -DHAVE_MRUBY_IO_GEM
            -DMRB_USE_RATIONAL
            -DMRB_USE_COMPLEX
            -DMRB_USE_BIGINT
            -DMRB_USE_DEBUG_HOOK
    )

    add_dependencies(${TARGET} ${TARGET}-build)

endmacro()