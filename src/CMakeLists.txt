include_directories(./)
if (EULER_MATH_BUILD)
    add_subdirectory(euler/math)
endif ()
if (EULER_PHYSICS_BUILD)
    add_subdirectory(euler/physics)
endif ()

add_subdirectory(euler/runtime)
add_subdirectory(euler/util)

if (EULER_DRAGONRUBY_BUILD)
    add_library(euler_dragonruby SHARED
            euler/dragonruby.cpp
    )
    target_link_libraries(euler_dragonruby PUBLIC dragonruby euler_runtime)

    target_compile_options(euler_util PUBLIC -fPIC)
    target_compile_options(euler_runtime PUBLIC -fPIC)
    if (EULER_MATH_BUILD)
        target_compile_options(euler_math PUBLIC -fPIC)
    endif ()
    if (EULER_PHYSICS_BUILD)
        target_compile_options(euler_physics PUBLIC -fPIC)
    endif ()
endif ()