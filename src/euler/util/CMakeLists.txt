add_library(euler_util STATIC
        color.cpp
        color.h
        error.cpp
        error.h
        image.cpp
        image.h
        logger.cpp
        logger.h
        ruby_state.cpp
        ruby_state.h
        object.cpp
        object.h
        state.cpp
        state.h
        storage.cpp
        storage.h
        system.cpp
        system.h
        ext.cpp
        ext.h
        math.cpp
        math.h
)

target_compile_definitions(euler_util PUBLIC
        ${EULER_DEFINITIONS}
)

target_compile_options(euler_util PUBLIC
        ${EULER_CFLAGS}
)

if (EULER_DRAGONRUBY_BUILD)
    target_link_libraries(euler_util PUBLIC
            dragonruby
    )

endif ()