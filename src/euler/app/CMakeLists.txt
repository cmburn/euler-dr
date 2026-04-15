add_library(euler_app STATIC
        state.cpp
        state.h
)

if (EULER_DRAGONRUBY_BUILD)
    target_sources(euler_app PUBLIC
            dragonruby/logger.cpp
            dragonruby/logger.h
            dragonruby/ruby_state.cpp
            dragonruby/ruby_state.h
            dragonruby/state.cpp
            dragonruby/state.h
            dragonruby/image.cpp
            dragonruby/image.h
            dragonruby/window.cpp
            dragonruby/window.h
            dragonruby/renderer.cpp
            dragonruby/renderer.h
    )
    target_link_libraries(euler_app PUBLIC
            dragonruby
    )
    target_include_directories(euler_app PUBLIC
            ${EULER_DRAGONRUBY_PATH}/include
    )
    target_compile_definitions(euler_app PUBLIC
            -DEULER_DRAGONRUBY
    )
endif ()

target_link_libraries(euler_app PUBLIC
        euler_util
)

if (EULER_MATH_BUILD)
    target_link_libraries(euler_app PUBLIC
            euler_math
    )
endif ()

if (EULER_PHYSICS_BUILD)
    target_link_libraries(euler_app PUBLIC
            euler_physics
    )
endif ()

if (EULER_GUI_BUILD)
    target_link_libraries(euler_app PUBLIC
            euler_gui
    )
endif ()

if (EULER_NET_BUILD)
    target_link_libraries(euler_app PUBLIC
            euler_net
    )
endif ()