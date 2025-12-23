# With a minimal amount of effort, a few other formats could be added here,
# namely ones included with glaze and libarchive, as they don't require any
# additional dependencies.

add_library(euler_file STATIC
        file.cpp
        file.h
        format.cpp
        format.h
        format/json.cpp
        format/json.h
        format/zip.cpp
        format/zip.h
)

target_link_libraries(euler_file PUBLIC
        glaze::glaze
)