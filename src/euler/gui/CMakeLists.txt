add_library(euler_gui STATIC
        button.cpp
        button.h
        checkbox.cpp
        checkbox.h
        ext.cpp
        ext.h
        color_picker.cpp
        color_picker.h
        internal.cpp
        internal.h
        common.cpp
        common.h
        dropdown.cpp
        dropdown.h
        element.cpp
        element.h
        window.cpp
        window.h
        knob.cpp
        knob.h
        progress_bar.cpp
        progress_bar.h
        radio_button.cpp
        radio_button.h
        row.cpp
        row.h
        selectable.cpp
        selectable.h
        slider.cpp
        slider.h
        style.cpp
        style.h
        widget.cpp
        widget.h
        text.cpp
        text.h
)

target_link_libraries(euler_gui PUBLIC
        euler_util
        euler_graphics
        mruby
)

target_compile_options(euler_gui PUBLIC
        ${EULER_CFLAGS}
)

target_compile_definitions(euler_gui PRIVATE
        ${EULER_DEFINITIONS}
)

target_link_libraries(euler_gui PRIVATE
        VK2D
)