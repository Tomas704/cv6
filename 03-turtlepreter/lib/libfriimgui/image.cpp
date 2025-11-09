#include "image.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include <stdexcept>

namespace friimgui {

Image Image::createImage(const std::filesystem::path &fileName) {
    int w, h, comp;
    unsigned char *data
        = stbi_load(fileName.string().c_str(), &w, &h, &comp, 4);
    if (! data) {
        throw std::runtime_error(
            "Failed to load image from: " + fileName.string()
        );
    }

    GLuint tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGBA,
        w,
        h,
        0,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        data
    );
    stbi_image_free(data);

    return Image(tex, w, h);
}

void Image::draw(
    const friimgui::Region &region,
    const Transformation &transformation
) {
    ImDrawList *drawList = ImGui::GetWindowDrawList();
    Region imageRegion = Region::createAtPosCenter(0, 0, m_width, m_height);

    imageRegion.scaleThis(transformation.scale);
    imageRegion.rotateThis(transformation.rotation);
    imageRegion.translateThis(transformation.translation);
    imageRegion.translateThis(region.getP0());

    drawList->AddImageQuad(
        // texture
        (ImTextureID)(intptr_t)m_tex,
        // vertices
        imageRegion.getP0(),
        imageRegion.getP1(),
        imageRegion.getP2(),
        imageRegion.getP3(),
        // tex coordinates
        ImVec2(0, 0),
        ImVec2(1, 0),
        ImVec2(1, 1),
        ImVec2(0, 1),
        // tint
        IM_COL32_WHITE
    );
}

Image::Image(GLuint tex, int width, int height) :
    m_tex(tex),
    m_width(width),
    m_height(height) {
}

} // namespace friimgui