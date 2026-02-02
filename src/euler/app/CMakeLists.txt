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
            dragonruby/storage.cpp
            dragonruby/storage.h
    )
    target_link_libraries(euler_app PUBLIC
            dragonruby
    )
    target_include_directories(euler_app PUBLIC
            ${EULER_DRAGONRUBY_PATH}/include
    )
endif ()

if (EULER_NATIVE_BUILD)
    target_sources(euler_app PUBLIC
            native/logger.cpp
            native/logger.h
            native/ruby_state.cpp
            native/ruby_state.h
            native/state.cpp
            native/state.h
            native/storage.cpp
            native/storage.h
    )

    target_include_directories(euler_app PUBLIC
            ${CMAKE_CURRENT_SOURCE_DIR}/../../
    )

    # if vulkan_sdk env var exists, use it instead of system headers
    if(DEFINED ENV{VULKAN_SDK})
        set(Vulkan_INCLUDE_DIR "$ENV{VULKAN_SDK}/include")
        include_directories(BEFORE SYSTEM $ENV{VULKAN_SDK}/include)
    endif()
endif ()

message(STATUS "Euler App Definitions: ${EULER_DEFINITIONS}")

target_compile_definitions(euler_app PUBLIC
        ${EULER_DEFINITIONS}
)

target_compile_options(euler_app PUBLIC
        ${EULER_CFLAGS}
)

#target_link_libraries(euler_app PUBLIC
#        euler_util
#        euler_math
#        # euler_physics
#)

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

target_link_libraries(euler_app PUBLIC
        euler_util
)