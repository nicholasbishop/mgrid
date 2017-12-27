#include <GLFW/glfw3.h>

#include "events.hh"

namespace mgrid {

CursorPositionEvent::CursorPositionEvent(const dvec2& pos) : pos(pos) {}

KeyEvent::KeyEvent(const int key,
                   const int scancode,
                   const int action,
                   const int mods)
    : key(key), scancode(scancode), action(action), mods(mods) {}

bool KeyEvent::isPress() const {
  return action == GLFW_PRESS || action == GLFW_REPEAT;
}

bool KeyEvent::isEscape() const {
  return key == GLFW_KEY_ESCAPE;
}

bool KeyEvent::isDownArrow() const {
  return key == GLFW_KEY_DOWN;
}

bool KeyEvent::isLeftArrow() const {
  return key == GLFW_KEY_LEFT;
}

bool KeyEvent::isRightArrow() const {
  return key == GLFW_KEY_RIGHT;
}

bool KeyEvent::isUpArrow() const {
  return key == GLFW_KEY_UP;
}

MouseButtonEvent::MouseButtonEvent(const int button,
                                   const int action,
                                   const int mods,
                                   const dvec2& pos)
    : button(button), action(action), mods(mods), pos(pos) {}

bool MouseButtonEvent::isLeftButton() const {
  return button == GLFW_MOUSE_BUTTON_LEFT;
}

bool MouseButtonEvent::isPress() const {
  return action == GLFW_PRESS;
}

}  // namespace mgrid
