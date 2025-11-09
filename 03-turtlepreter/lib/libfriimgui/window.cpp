#include "window.hpp"

#include <iostream>

namespace friimgui {

Window *Window::initializeWindow(size_t width, size_t height) {
    if (Window::k_instance == nullptr) {
        // 1. Initialize GLFW
        if (! glfwInit()) {
            std::cerr << "Failed to initialize GLFW\n";
            return nullptr;
        }

        // 2. Create window + OpenGL context
        GLFWwindow *window = glfwCreateWindow(width, height, "App", NULL, NULL);
        if (! window) {
            std::cerr << "Failed to create GLFW window\n";
            glfwTerminate();
            return nullptr;
        }
        glfwMakeContextCurrent(window);

        // 3. Load OpenGL functions with GLAD
        if (! gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            std::cerr << "Failed to initialize GLAD\n";
            return nullptr;
        }

        // 4. Setup Dear ImGui context and  style
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGui::StyleColorsDark(); // or StyleColorsClassic / StyleColorsLight

        // 5. Setup Platform/Renderer bindings
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init();

        // 6. Create instance
        Window::k_instance = new Window(window);
    }
    return Window::k_instance;
}

void Window::releaseWindow() {
    if (Window::k_instance != nullptr) {
        // 1. Cleanup Dear ImGui
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

        // 2. Cleanup GLFW
        glfwDestroyWindow(Window::k_instance->m_GLFWwindow);
        glfwTerminate();

        // 3. Release instance
        delete Window::k_instance;
        Window::k_instance = nullptr;
    }
}

void friimgui::Window::setGUI(GUIBuilder *GUIBuilder) {
    m_GUIBuilder = GUIBuilder;
}

void Window::run() {
    while (! glfwWindowShouldClose(m_GLFWwindow)) {
        glfwPollEvents();

        // 1. Start new ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // 2. Build GUI
        if (m_GUIBuilder != nullptr) {
            m_GUIBuilder->build();
        }

        // 3. Render GUI
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(m_GLFWwindow, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(m_GLFWwindow);
    }
}

Window::Window(GLFWwindow *glfwWindow) :
    m_GLFWwindow(glfwWindow),
    m_GUIBuilder(nullptr) {
    // io = ImGui::GetIO();
    // io.ConfigFlags |= ImGuiConfigFlags_DockingEnable; // optional
    // io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // optional
}

Window::~Window() {
    m_GLFWwindow = nullptr;
    m_GUIBuilder = nullptr;
}

Window *Window::k_instance = nullptr;

} // namespace friimgui