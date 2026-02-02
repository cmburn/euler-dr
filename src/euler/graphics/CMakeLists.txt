add_library(euler_graphics STATIC
        animated_sprite.cpp
        animated_sprite.h
        basic_sprite.cpp
        basic_sprite.h
        camera.cpp
        camera.h
        composite_sprite.cpp
        composite_sprite.h
        ext.cpp
        ext.h
        font.cpp
        font.h
        image.cpp
        image.h
        polygon.cpp
        polygon.h
        sprite.cpp
        sprite.h
        text.cpp
        text.h
        window.cpp
        window.h
)

target_compile_options(euler_graphics PUBLIC
        ${EULER_CFLAGS}
)

target_compile_definitions(euler_graphics PUBLIC
        ${EULER_DEFINITIONS}
)

target_link_libraries(euler_graphics PUBLIC
        euler_util
        euler_vulkan
        VK2D
        armadillo
        mruby
)

