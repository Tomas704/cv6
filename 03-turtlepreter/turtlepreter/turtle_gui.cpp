#include "turtle_gui.hpp"

#include <libfriimgui/types.hpp>

#include <imgui/imgui.h>

namespace turtlepreter {

TurtleGUI::TurtleGUI(Turtle *turtle, Interpreter *interpreter)
    : m_turtle(turtle), m_interpreter(interpreter), m_widthLeftPanel(200) {}

void TurtleGUI::build() {
  ImGuiIO &io = ImGui::GetIO();

  ImGui::SetNextWindowPos(ImVec2(0, 0));
  ImGui::SetNextWindowSize(io.DisplaySize);

  ImGui::Begin("Turtlepreter", nullptr,
               ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                   ImGuiWindowFlags_NoCollapse);

  buildTopBar();
  buildLeftPanel();
  ImGui::SameLine();
  buildSplitter();
  ImGui::SameLine();
  buildRightPanel();

  ImGui::End();
}

void TurtleGUI::buildTopBar() {
  if (ImGui::Button("Run", ImVec2(100, 0))) {
    m_interpreter->interpretAll(*m_turtle);
  }
  ImGui::SameLine();
  if (ImGui::Button("Step", ImVec2(100, 0))) {
    m_interpreter->interpretStep(*m_turtle);
  }
  ImGui::SameLine();
  if (ImGui::Button("Reset", ImVec2(100, 0))) {
    m_turtle->reset();
    m_interpreter->reset();
  }
}

void TurtleGUI::buildLeftPanel() {
  ImGui::BeginChild("Script", ImVec2(m_widthLeftPanel, 0), true);
  populateTreeNodes(m_interpreter->getRoot());
  ImGui::EndChild();
}

void TurtleGUI::buildSplitter() {
  ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.5f, 0.5f, 0.5f, 1.0f));
  ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.7f, 0.7f, 0.7f, 1.0f));
  ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.9f, 0.9f, 0.9f, 1.0f));
  ImGui::Button("|", ImVec2(5, ImGui::GetContentRegionAvail().y));
  if (ImGui::IsItemActive()) {
    m_widthLeftPanel += ImGui::GetIO().MouseDelta.x;
    if (m_widthLeftPanel < 100) {
      m_widthLeftPanel = 100; // minimum size
    }
  }
  ImGui::PopStyleColor(3);
}

void TurtleGUI::buildRightPanel() {
  ImGui::BeginChild("Turtle", ImVec2(0, 0), true);

  ImDrawList *drawList = ImGui::GetWindowDrawList();
  friimgui::Region region = friimgui::Region::createFromAvail();
  drawList->AddRectFilled(region.getP0(), region.getP2(),
                          IM_COL32(60, 60, 60, 255));
  region.reserveSpace();

  m_turtle->draw(region);

  ImGui::EndChild();
}

void TurtleGUI::populateTreeNodes(Node *node) {
  if (node != nullptr) {
    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen;

    /**
     * Reťazec reprezentujúci vrchol, ktorý sa zobrazí v okne.
     * Túto premennú je potrebné vhodne inicializovať tak,
     * aby obsahovala textovú reprezentáciu parametra node.
     */
    const std::string nodeStr = node->toString();
    if (ImGui::TreeNodeEx(node, flags, "%s", nodeStr.c_str())) {

      for (Node *subnode : node->getSubnodes()) {
        populateTreeNodes(subnode);
      }

      ImGui::TreePop();
    }
  }
}

} // namespace turtlepreter
