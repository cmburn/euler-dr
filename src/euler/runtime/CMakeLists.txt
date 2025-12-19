add_library(euler_runtime STATIC
        state.cpp
        state.h
)

if (EULER_DRAGONRUBY_BUILD)
#    add sources to euler_runtime
    target_sources(euler_runtime PUBLIC
            dragonruby/state.cpp
            dragonruby/state.h
            dragonruby/mruby_state.cpp
            dragonruby/mruby_state.h
    )
    target_link_libraries(euler_runtime PUBLIC
            dragonruby
    )
endif()

target_link_libraries(euler_runtime PUBLIC
        euler_util
        euler_math
        euler_physics
)