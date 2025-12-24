add_library(euler_util STATIC
        color.cpp
        color.h
        error.cpp
        error.h
        image.cpp
        image.h
        logger.cpp
        logger.h
        mruby_state.cpp
        mruby_state.h
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
)

target_link_libraries(euler_util PUBLIC
        dragonruby
)

target_compile_definitions(euler_util PUBLIC EULER_DRAGONRUBY)