add_library(euler_graphics STATIC
        target.cpp
        target.h
        user_interface.cpp
        user_interface.h
)

target_link_libraries(euler_graphics PUBLIC
        euler_util
        euler_math
        eigen
        nuklear
)
