#ifndef SRC_WINDOW_HH_
#define SRC_WINDOW_HH_

#include "common.hh"

struct GLFWwindow;

namespace mgrid {

class GLVersion {
public:
  GLVersion(const int major, const int minor);

  const int major;
  const int minor;
};

class KeyEvent {
 public:
  KeyEvent(const int key, const int scancode, const int action,
           const int mods);

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

class Size2i {
 public:
  Size2i(const int width, const int height);

  const int width;
  const int height;
};

class Window {
 public:
  Window(const GLVersion& version);

  void close();

  void start();

  Size2i framebuffer_size() const;

  float aspect_ratio() const;

 private:
  virtual void on_key_event(const KeyEvent& event);

  virtual void initialize() = 0;

  virtual void render() = 0;

  virtual void clean_up() = 0;

  static void key_callback(GLFWwindow* window, const int key,
                           const int scancode, const int action,
                           const int mode);

  GLFWwindow* wnd_;
  bool is_initialized_ = false;
};

}

#endif  // SRC_WINDOW_HH_
