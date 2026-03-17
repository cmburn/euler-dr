add_library(euler_util STATIC
        color.cpp
        color.h
        error.cpp
        error.h
        ext.cpp
        ext.h
        image.cpp
        image.h
        logger.cpp
        logger.h
        math.cpp
        math.h
        object.cpp
        object.h
        ruby_state.cpp
        ruby_state.h
        state.cpp
        state.h
        types.cpp
        types.h
        version.cpp
        version.h
        window.cpp
        window.h
)

target_compile_definitions(euler_util PUBLIC
        ${EULER_DEFINITIONS}
)

target_compile_options(euler_util PUBLIC
        ${EULER_CFLAGS}
)

target_link_libraries(euler_util PUBLIC
        eigen
)

# TODO: not this
target_include_directories(euler_util PUBLIC
        ${CMAKE_CURRENT_BINARY_DIR}/../../_deps/eigen-src
)

if (EULER_DRAGONRUBY_BUILD)
    target_link_libraries(euler_util PUBLIC
            dragonruby
    )
endif ()

if (EULER_NATIVE_BUILD)
    target_link_libraries(euler_util PUBLIC
            mruby
            SDL3-static
    )
endif ()

if (EULER_GUI_BUILD)
    target_link_libraries(euler_util PUBLIC
            nuklear
    )
    target_sources(euler_util PUBLIC
            nuklear.h
    )
endif ()