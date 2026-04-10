add_library(euler_graphics STATIC
        renderer.cpp
        renderer.h
)

target_link_libraries(euler_graphics PUBLIC
        euler_util
        euler_math
        eigen
)