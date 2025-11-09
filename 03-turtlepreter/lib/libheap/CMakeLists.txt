add_library(heap STATIC)

target_sources(heap PRIVATE heap_monitor.cpp)

target_compile_options(heap PRIVATE
    -Wall
    -Wextra
    -Wpedantic
    -std=c++20
)

target_include_directories(heap PUBLIC .)
