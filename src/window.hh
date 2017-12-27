#ifndef SRC_WINDOW_HH_
#define SRC_WINDOW_HH_

#include "glm/vec2.hpp"

#include "common.hh"

struct GLFWwindow;

namespace mgrid {

class GLVersion {
 public:
  GLVersion(const int major, const int minor);

  const int major;
  const int minor;
};

class CursorPositionEvent {
 public:
  CursorPositionEvent(const vec2& pos);

  const vec2 pos;
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
  MouseButtonEvent(const int button, const int action, const int mods);

  const int button;
  const int action;
  const int mods;
};

class Window {
 public:
  Window(const GLVersion& version);

  void close();

  void start();

  ivec2 framebuffer_size() const;

 private:
  virtual void on_cursor_position_event(const CursorPositionEvent& event);
  virtual void on_key_event(const KeyEvent& event);
  virtual void on_mouse_button_event(const MouseButtonEvent& event);

  virtual void initialize() = 0;

  virtual void render() = 0;

  virtual void clean_up() = 0;

  static void cursor_pos_callback(GLFWwindow* const window,
                                  const double xpos,
                                  const double ypos);

  static void key_callback(GLFWwindow* const window,
                           const int key,
                           const int scancode,
                           const int action,
                           const int mode);

  static void mouse_button_callback(GLFWwindow* const window,
                                    const int button,
                                    const int action,
                                    const int mods);

  static Window* from_user_pointer(GLFWwindow* const window);

  GLFWwindow* wnd_;
  bool is_initialized_ = false;
};
}

#endif  // SRC_WINDOW_HH_
