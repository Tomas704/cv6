#ifndef TURTLEPRETER_INTERPRETER_HPP
#define TURTLEPRETER_INTERPRETER_HPP

#include "turtle.hpp"

#include <string>
#include <vector>

namespace turtlepreter {

class ICommand;
class Cursor;

class Node {
public:
  static Node *createLeafNode(ICommand *command);
  static Node *createSequentialNode();
  ~Node();
  std::string toString() const;
  void addSubnode(Node *subnode);
  Node *getParent() const;
  const std::vector<Node *> &getSubnodes() const;
  ICommand *getCommand() const;
  Cursor *getCursor() const;
  void setIsFocused(bool isFocused);

private:
  Node(ICommand *command, Cursor *cursor);
  Node *m_parent;
  std::vector<Node *> m_subnodes;
  Cursor *m_cursor;
  ICommand *m_command;
  bool m_isFocused;
};

// ==================================================

class Cursor {
public:
  Cursor();
  virtual ~Cursor() = default;
  virtual Node *next() = 0;
  virtual void reset() = 0;
  virtual std::string toString() const = 0;
  void setNode(Node *node);

protected:
  Node *m_node;
};

class CursorUp : public Cursor {
public:
  CursorUp() = default;
  Node *next() override;
  void reset() override;
  std::string toString() const override;
};

class SequentialCursor : public Cursor {
public:
  SequentialCursor();
  Node *next() override;
  void reset() override;
  std::string toString() const override;

private:
  int m_current;
};

class ICommand {
public:
  virtual ~ICommand() = default;

  /**
   * Vykoná akciu s korytnačkou @p turtle
   */
  virtual void execute(Turtle &turtle) = 0;

  /**
   * Vráti reťazec popisujúci príkaz.
   */
  virtual std::string toString() = 0;
};

// --------------------------------------------------

/**
 * Posunie korytnačku o zadanú vzdialenosť v smere jej natočenia.
 */
class CommandMove : public ICommand {
public:
  CommandMove(float d);

  void execute(Turtle &turtle) override;
  std::string toString() override;

private:
  float m_d;
};

// --------------------------------------------------

/**
 * Presunie korytnačku na zadané súradnice.
 */
class CommandJump : public ICommand {
public:
  CommandJump(float x, float y);

  void execute(Turtle &turtle) override;
  std::string toString() override;

private:
  float m_x;
  float m_y;
};

// --------------------------------------------------

/**
 * Zmení natočenie korytnačky.
 */
class CommandRotate : public ICommand {
public:
  CommandRotate(float angleRad);

  void execute(Turtle &turtle) override;
  std::string toString() override;

private:
  float m_angleRad;
};

// ==================================================

class Interpreter {
public:
  Interpreter(Node *root);
  void interpretAll(Turtle &turtle);
  void interpretStep(Turtle &turtle);
  void reset();
  bool wasSomethingExecuted();
  bool isFinished() const;
  Node *getRoot() const;
  Node *getCurrent() const;
  bool stopOnNodeWithoutCommand();
  void setStopOnNodeWithoutCommand(bool value);

private:
  Node *m_root;
  Node *m_current;
  void interpterSubtreeNodes(Node *node, Turtle &turtle);
  void moveCurrent();
  void resetCursorsRecursive(Node *node);
};

} // namespace turtlepreter

#endif
