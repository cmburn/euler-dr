add_library(euler_math STATIC
        cube.cpp
        cube.h
        ext.cpp
        ext.h
        functions.cpp
        functions.h
        matrix.cpp
        matrix.h
        nonscalar.cpp
        nonscalar.h
        impl.cpp
        impl.h
        row_vector.cpp
        row_vector.h
        size.cpp
        size.h
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