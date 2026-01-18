# TODO

add_executable(euler_math_matrix_test
        matrix_test.cpp
)

target_link_libraries(euler_math_matrix_test
        PRIVATE
        euler_app
        GTest::gtest_main
)
