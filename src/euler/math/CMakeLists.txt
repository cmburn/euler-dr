add_library(euler_math STATIC
        ext.cpp
        ext.h
        row_vector.cpp
        row_vector.h
        sparse_matrix.cpp
        sparse_matrix.h
        util.cpp
        util.h
)

target_link_libraries(euler_math PUBLIC
        euler_util
        armadillo
        mruby
)

target_compile_definitions(euler_math PUBLIC EULER_MATH)

target_compile_options(euler_math PRIVATE
        -Wno-changes-meaning
)