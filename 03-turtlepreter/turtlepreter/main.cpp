#include "interpreter.hpp"
#include "turtle.hpp"
#include "turtle_gui.hpp"

#include <libfriimgui/window.hpp>

#include <imgui/imgui.h>

#include <string>

#include "heap_monitor.hpp"

int main(int argc, char *argv[]) {
  namespace tp = turtlepreter;

  (void)argc;
  (void)argv;

  const std::string cTurtleImg = ".\\turtlepreter\\resources\\turtle.png";
  const int cWidth = 1280;
  const int cHeight = 720;
  const int cCenterX = cWidth / 2;
  const int cCenterY = cHeight / 2;

  // Create window, make OpenGL available.
  friimgui::Window *window =
      friimgui::Window::initializeWindow(cWidth, cHeight);

  // Create the turtle.
  tp::Turtle turtle(cTurtleImg, cCenterX, cCenterY);

  tp::CommandJump *jump1 = new tp::CommandJump(cCenterX, cCenterY + 100);
  tp::CommandJump *jump2 = new tp::CommandJump(cCenterX + 100, cCenterY + 100);

  tp::Node *root = tp::Node::createSequentialNode();
  tp::Node *son1 = tp::Node::createLeafNode(jump1);
  tp::Node *son2 = tp::Node::createLeafNode(jump2);

  root->addSubnode(son1);
  root->addSubnode(son2);

  // Interpret the tree from the root.
  tp::Interpreter interpreter(root);

  // Create GUI.
  tp::TurtleGUI turtleGUI(&turtle, &interpreter);

  // Render GUI.
  window->setGUI(&turtleGUI);
  window->run();
  friimgui::Window::releaseWindow();

  delete root;
  // delete jump1;
  // delete jump2;
}
