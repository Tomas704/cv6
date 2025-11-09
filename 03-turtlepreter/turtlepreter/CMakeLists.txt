add_executable(turtlepreter)

target_sources(turtlepreter PRIVATE
    interpreter.cpp
    turtle.cpp
    turtle_gui.cpp
    main.cpp
)

target_compile_options(turtlepreter PRIVATE
    -Wall
    -Wextra
    -Wpedantic
    -std=c++20
)

target_link_libraries(turtlepreter PRIVATE friimgui heap)

add_custom_command(
    TARGET turtlepreter POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different 
        "${PROJECT_SOURCE_DIR}/resources/"
        "$<TARGET_FILE_DIR:turtlepreter>/resources/"
    COMMENT "Copy resources"
)
