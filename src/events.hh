#ifndef SRC_EVENTS_HH_
#define SRC_EVENTS_HH_

#include "glm/vec2.hpp"

#include "common.hh"

namespace mgrid {

class CursorPositionEvent {
 public:
  CursorPositionEvent(const dvec2& pos);

  const dvec2 pos;
};

class KeyEvent {
 public:
  KeyEvent(const int key, const int scancode, const int action, const int mods);

  bool isPress() const;

  bool isEscape() const;

  bool isDownArrow() const;
  bool isLeftArrow() const;
  bool isRightArrow() const;
  bool isUpArrow() const;

  const int key;
  const int scancode;
  const int action;
  const int mods;
};

class MouseButtonEvent {
 public:
  MouseButtonEvent(const int button,
                   const int action,
                   const int mods,
                   const dvec2& pos);

  bool isLeftButton() const;
  bool isPress() const;

  const int button;
  const int action;
  const int mods;
  const dvec2 pos;
};
}

#endif  // SRC_EVENTS_HH_
