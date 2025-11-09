#ifndef FRIIMGUI_IMAGE_HPP
#define FRIIMGUI_IMAGE_HPP

#include "types.hpp"

#include <glad/glad.h>

#include <imgui/imgui.h>

#include <filesystem>

namespace friimgui {

class Image {
public:
    static Image createImage(const std::filesystem::path &fileName);

    void draw(
        const friimgui::Region &region,
        const Transformation &transformation
    );

private:
    Image(GLuint tex, int width, int height);

    GLuint m_tex;
    int m_width;
    int m_height;
};

} // namespace friimgui

#endif
