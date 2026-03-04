add_library(euler_math STATIC
        matrix.cpp
        matrix.h
)

target_link_libraries(euler_math PUBLIC
        euler_util
        eigen
)

target_compile_definitions(euler_math PUBLIC
        ${EULER_DEFINITIONS}
)

target_compile_options(euler_math PRIVATE
        -Wno-changes-meaning
)

target_compile_options(euler_math PUBLIC
        ${EULER_CFLAGS}
)