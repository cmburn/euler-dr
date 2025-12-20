add_library(euler_math STATIC
        cube.cpp
        cube.h
        ext.cpp
        ext.h
        fixed.cpp
        fixed.h
        matrix.cpp
        matrix.h
        nonscalar.cpp
        nonscalar.h
        row_vector.cpp
        row_vector.h
        sparse_matrix.cpp
        sparse_matrix.h
        util.cpp
        util.h
        vector.cpp
        vector.h
)

target_link_libraries(euler_math PUBLIC
        euler_util
        armadillo
        mruby
)

target_compile_definitions(euler_math PUBLIC
    EULER_MATH_MATRIX_ROW_LIMIT=${FIXED_MATRIX_ROW_LIMIT}
    EULER_MATH_MATRIX_COLUMN_LIMIT=${FIXED_MATRIX_COLUMN_LIMIT}
)

target_compile_options(euler_math PRIVATE
        -Wno-changes-meaning
)