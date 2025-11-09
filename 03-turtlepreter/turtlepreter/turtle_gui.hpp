#ifndef TURTLEPRETER_TURTLE_GUI_HPP
#define TURTLEPRETER_TURTLE_GUI_HPP

#include "interpreter.hpp"
#include "turtle.hpp"

#include <libfriimgui/gui_builder.hpp>

namespace turtlepreter {

class TurtleGUI : public friimgui::GUIBuilder {
public:
    TurtleGUI(Turtle *turtle, Interpreter *interpreter);

    void build() override;

private:
    void buildTopBar();
    void buildLeftPanel();
    void buildSplitter();
    void buildRightPanel();
    void populateTreeNodes(Node *node);

    Turtle *m_turtle;
    Interpreter *m_interpreter;
    size_t m_widthLeftPanel;
};

} // namespace turtlepreter

#endif