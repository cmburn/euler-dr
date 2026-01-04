add_library(euler_app STATIC
        state.cpp
        state.h
)

if (EULER_DRAGONRUBY_BUILD)
    target_sources(euler_app PUBLIC
            dragonruby/logger.cpp
            dragonruby/logger.h
            dragonruby/mruby_state.cpp
            dragonruby/ruby_state.h
            dragonruby/state.cpp
            dragonruby/state.h
            dragonruby/storage.cpp
            dragonruby/storage.h
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

if (EULER_NATIVE_BUILD)
    target_sources(euler_app PUBLIC
            native/logger.cpp
            native/logger.h
            native/mruby_state.cpp
            native/mruby_state.h
            native/state.cpp
            native/state.h
            native/storage.cpp
            native/storage.h
    )
    target_compile_definitions(euler_app PUBLIC
            -DEULER_NATIVE
    )

    # if vulkan_sdk env var exists, use it instead of system headers
    if(DEFINED ENV{VULKAN_SDK})
        set(Vulkan_INCLUDE_DIR "$ENV{VULKAN_SDK}/include")
        include_directories(BEFORE SYSTEM $ENV{VULKAN_SDK}/include)
    endif()
endif ()

target_link_libraries(euler_app PUBLIC
        euler_util
        euler_math
        euler_physics
)