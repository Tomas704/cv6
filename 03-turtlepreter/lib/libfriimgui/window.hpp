#ifndef FRIIMGUI_WINDOW_HPP
#define FRIIMGUI_WINDOW_HPP

#include <libfriimgui/gui_builder.hpp>

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/imgui.h>

namespace friimgui {

class Window {
public:
    static Window *initializeWindow(size_t width, size_t height);
    static void releaseWindow();

    void setGUI(GUIBuilder *GUIBuilder);

    void run();

private:
    Window(GLFWwindow *glfwWindow);
    ~Window();

    GLFWwindow *m_GLFWwindow;
    GUIBuilder *m_GUIBuilder;

    static Window *k_instance;
};

} // namespace friimgui

#endif