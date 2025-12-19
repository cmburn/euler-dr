include_directories(./)
add_subdirectory(euler/math)
add_subdirectory(euler/physics)
add_subdirectory(euler/runtime)
add_subdirectory(euler/util)

add_library(euler_dragonruby SHARED
        euler/dragonruby.cpp
)

target_link_libraries(euler_dragonruby PUBLIC dragonruby euler_runtime)
