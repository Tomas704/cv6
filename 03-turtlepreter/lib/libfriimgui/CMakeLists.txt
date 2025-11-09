add_library(friimgui STATIC
    types.cpp
    image.cpp
    gui_builder.cpp
    window.cpp
)

target_compile_options(friimgui PRIVATE
    -Wall
    -Wextra
    -Wpedantic
    -std=c++20
)

target_include_directories(friimgui PUBLIC ..)

target_link_libraries(friimgui PUBLIC
    imgui
    opengl32
    stb
)
