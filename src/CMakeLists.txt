include_directories(./)
set(EULER_CFLAGS
        -Wall
        -Wextra
        -Wno-class-memaccess # necessary due to some util::Reference trickery
)
set(EULER_DEFINITIONS
        -DEULER_UTIL
)

if (EULER_DRAGONRUBY_BUILD)
    list(APPEND EULER_CFLAGS -fPIC)
    list(APPEND EULER_DEFINITIONS -DEULER_DRAGONRUBY)
elseif (EULER_NATIVE_BUILD)
    list(APPEND EULER_DEFINITIONS
            -DEULER_NATIVE
            -DEULER_GRAPHICS
            -DEULER_GUI
            -DEULER_VULKAN
    )
endif ()

if (EULER_MATH_BUILD)
    list(APPEND EULER_DEFINITIONS -DEULER_MATH)
endif ()
if (EULER_PHYSICS_BUILD)
    list(APPEND EULER_DEFINITIONS -DEULER_PHYSICS)
endif ()

if (EULER_MATH_BUILD)
    add_subdirectory(euler/math)
endif ()
if (EULER_PHYSICS_BUILD)
    add_subdirectory(euler/physics)
endif ()

add_compile_definitions(EULER_INTERNAL_BUILD)

add_subdirectory(euler/app)
add_subdirectory(euler/util)

if (EULER_DRAGONRUBY_BUILD)
    list(APPEND EULER_DEFINITIONS -DEULER_DRAGONRUBY)
    add_compile_definitions(-DEULER_DRAGONRUBY)
    include_directories(${EULER_DRAGONRUBY_PATH}/include)
    add_library(euler_dragonruby SHARED
            euler/dragonruby.cpp
    )
    target_link_libraries(euler_dragonruby PUBLIC
            dragonruby
            euler_app
    )
    target_compile_definitions(euler_dragonruby PUBLIC
            ${EULER_DEFINITIONS}
    )
endif ()

if (EULER_NATIVE_BUILD)
    add_subdirectory(euler/event)
    add_subdirectory(euler/graphics)
    add_subdirectory(euler/gui)
    add_subdirectory(euler/vulkan)
endif ()