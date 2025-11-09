#include "interpreter.hpp"

#include "heap_monitor.hpp"

namespace turtlepreter {

Node *Node::createLeafNode(ICommand *command) {
  Cursor *cursor = new CursorUp();
  Node *leaf = new Node(command, cursor);
  cursor->setNode(leaf);
  return leaf;
}

Node *Node::createSequentialNode() {
  Cursor *cursor = new SequentialCursor();
  Node *sequential = new Node(nullptr, cursor);
  cursor->setNode(sequential);
  return sequential;
}

Node::Node(ICommand *command, Cursor *cursor)
    : m_parent(nullptr), m_cursor(cursor), m_command(command),
      m_isFocused(false) {}

Node::~Node() {
  for (Node *subnode : this->m_subnodes) {
    delete subnode;
  }
  delete m_cursor;
  delete m_command;
}

std::string Node::toString() const {
  std::string commandString =
      m_command == nullptr ? "No command" : "Command: " + m_command->toString();
  std::string cursorString =
      m_cursor == nullptr ? "No cursor" : "Cursor: " + m_cursor->toString();
  return commandString + " " + cursorString;
}

void Node::addSubnode(Node *subnode) {
  if (subnode != nullptr) {
    this->m_subnodes.push_back(subnode);
    subnode->m_parent = this;
  }
}

Node *Node::getParent() const { return this->m_parent; }

const std::vector<Node *> &Node::getSubnodes() const {
  return this->m_subnodes;
}

void Node::setIsFocused(bool isFocused) { m_isFocused = isFocused; }

Cursor *Node::getCursor() const { return m_cursor; }

Cursor::Cursor() : m_node(nullptr) {}

void Cursor::setNode(Node *node) { m_node = node; }

Node *CursorUp::next() {
  if (m_node != nullptr) {
    return m_node->getParent();
  }
  return nullptr;
}

void CursorUp::reset() {}

std::string CursorUp::toString() const { return "Up"; }

SequentialCursor::SequentialCursor() : m_current(-1) {}

Node *SequentialCursor::next() {
  if (m_node == nullptr) {
    return nullptr;
  }
  m_current++;
  int size = static_cast<int>(m_node->getSubnodes().size());
  if (m_current >= size) {
    return m_node->getParent();
  } else {
    return m_node->getSubnodes()[m_current];
  }
}

void SequentialCursor::reset() { m_current = -1; }

std::string SequentialCursor::toString() const {
  if (m_node == nullptr) {
    return "Invalid state!";
  }
  int size = static_cast<int>(m_node->getSubnodes().size());
  if (m_current + 1 >= size) {
    return "Up";
  } else {
    return "Child " + std::to_string(m_current + 2) + "/" +
           std::to_string(size);
  }
}

ICommand *Node::getCommand() const { return this->m_command; }

CommandMove::CommandMove(float d) : m_d(d) {}

void CommandMove::execute(Turtle &turtle) { turtle.move(this->m_d); }

std::string CommandMove::toString() {
  return "Move(" + std::to_string(m_d) + ")";
}

CommandJump::CommandJump(float x, float y) : m_x(x), m_y(y) {}

void CommandJump::execute(Turtle &turtle) { turtle.jump(this->m_x, this->m_y); }

std::string CommandJump::toString() {
  return "Jump(" + std::to_string(m_x) + ", " + std::to_string(m_y) + ")";
}

CommandRotate::CommandRotate(float angleRad) : m_angleRad(angleRad) {}

void CommandRotate::execute(Turtle &turtle) { turtle.rotate(this->m_angleRad); }

std::string CommandRotate::toString() {
  return "Rotate(" + std::to_string(m_angleRad) + ")";
}

Interpreter::Interpreter(Node *root) : m_root(root), m_current(root) {}

void Interpreter::interpretAll(Turtle &turtle) {
  while (m_current != nullptr) {
    interpretStep(turtle);
  }
}

void Interpreter::interpretStep(Turtle &turtle) {
  if (m_current == nullptr) {
    return;
  }
  ICommand *command = m_current->getCommand();
  if (command != nullptr) {
    command->execute(turtle);
  }
  moveCurrent();
}

void Interpreter::moveCurrent() {
  Cursor *cursor = m_current->getCursor();
  if (cursor == nullptr) {
    return;
  }
  Node *nextNode = cursor->next();
  if (nextNode != nullptr) {
    m_current = nextNode;
  } else {
    m_current = nullptr;
  }
}

Node *Interpreter::getRoot() const { return m_root; }

void Interpreter::interpterSubtreeNodes(Node *node, Turtle &turtle) {
  if (node == nullptr) {
    return;
  }
  ICommand *command = node->getCommand();
  if (command != nullptr) {
    command->execute(turtle);
  }
  for (Node *subnode : node->getSubnodes()) {
    interpterSubtreeNodes(subnode, turtle);
  }
}

void Interpreter::resetCursorsRecursive(Node *node) {
  if (node == nullptr) {
    return;
  }
  Cursor *cursor = node->getCursor();
  if (cursor != nullptr) {
    cursor->reset();
  }
  for (Node *subnode : node->getSubnodes()) {
    resetCursorsRecursive(subnode);
  }
}

void Interpreter::reset() {
  m_current = m_root;
  resetCursorsRecursive(m_root);
}

bool Interpreter::wasSomethingExecuted() {
  // TODO
  return false;
}

bool Interpreter::isFinished() const {
  // TODO
  return false;
}

Node *Interpreter::getCurrent() const {
  // TODO
  return nullptr;
}

bool Interpreter::stopOnNodeWithoutCommand() {
  // TODO
  return false;
}

void Interpreter::setStopOnNodeWithoutCommand([[maybe_unused]] bool value) {
  // TODO
}

} // namespace turtlepreter
