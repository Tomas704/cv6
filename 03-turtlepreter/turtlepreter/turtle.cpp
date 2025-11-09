#include "turtle.hpp"
#include <cmath>
#include <imgui/imgui.h>

#include <stdexcept>

namespace turtlepreter {

Turtle::Turtle(const std::string &imgPath)
    : m_transformation(), m_image(friimgui::Image::createImage(imgPath)),
      m_initialTransformation() {}

Turtle::Turtle(const std::string &imgPath, float centerX, float centerY)
    : Turtle(imgPath) {
  m_transformation.translation.setValue({centerX, centerY});
  m_initialTransformation = m_transformation;
}

void Turtle::draw(const friimgui::Region &region) {
  const float thickness = 1;
  const ImColor color = ImColor(ImVec4({0, 1, 0, 1}));
  ImDrawList *drawList = ImGui::GetWindowDrawList();
  const ImVec2 p0 = region.getP0();
  for (const ImVec4 &segment : m_path) {
    ImVec2 start = {segment.x + p0.x, segment.y + p0.y};
    ImVec2 dest = {segment.z + p0.x, segment.w + p0.y};
    drawList->AddLine(start, dest, color, thickness);
  }
  m_image.draw(region, m_transformation);
}

void Turtle::reset() {
  m_path.clear();
  m_transformation = m_initialTransformation;
}

void Turtle::move(float distance) {
  const ImVec2 pos = m_transformation.translation.getValueOrDef();
  const float angleRad = m_transformation.rotation.getValueOrDef();
  const float delta_x = distance * cosf(angleRad);
  const float delta_y = distance * sinf(angleRad);
  const ImVec2 dest = {pos.x + delta_x, pos.y + delta_y};
  const ImVec4 path(pos.x, pos.y, dest.x, dest.y);
  m_path.push_back(path);
  m_transformation.translation.setValue(dest);
}

void Turtle::jump(float x, float y) {
  const ImVec2 dest(x, y);
  const ImVec2 pos = m_transformation.translation.getValueOrDef();
  const ImVec4 path(pos.x, pos.y, dest.x, dest.y);
  m_path.push_back(path);
  m_transformation.translation.setValue(dest);
}

void Turtle::rotate(float angleRad) {
  m_transformation.rotation.setValue(angleRad);
}

} // namespace turtlepreter
