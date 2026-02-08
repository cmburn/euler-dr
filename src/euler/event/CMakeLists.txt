add_library(euler_event STATIC
        event.cpp
        event.h
)

target_link_libraries(euler_event PUBLIC
        euler_util
)