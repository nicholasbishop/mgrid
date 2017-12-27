#include <GLFW/glfw3.h>

#include "window.hh"

namespace {

void error_callback(int error, const char *description) {
  fprintf(stderr, "Error %d: %s\n", error, description);
}

void init_glfw() {
  glfwSetErrorCallback(error_callback);
  if (!glfwInit())
    exit(EXIT_FAILURE);
}

static GLFWwindow *create_window(const mgrid::GLVersion &version) {
  init_glfw();

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, version.major);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, version.minor);
  GLFWwindow *window = glfwCreateWindow(1280, 800, "mgrid", NULL, NULL);
  if (!window) {
    glfwTerminate();
    exit(EXIT_FAILURE);
  }
  return window;
}

}

namespace mgrid {

GLVersion::GLVersion(const int major, const int minor)
    : major(major), minor(minor) {
}

KeyEvent::KeyEvent(const int key, const int scancode, const int action,
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

Size2i::Size2i(const int width, const int height)
    : width(width), height(height) {
}

Window::Window(const GLVersion& version)
    : wnd_(create_window(version)) {
  glfwSetWindowUserPointer(wnd_, this);
  glfwSetKeyCallback(wnd_, &Window::key_callback);

  glfwMakeContextCurrent(wnd_);
  glfwSwapInterval(1);
}

void Window::close() {
  glfwSetWindowShouldClose(wnd_, true);
}

void Window::start() {
  initialize();

  while (!glfwWindowShouldClose(wnd_)) {
    render();

    glfwSwapBuffers(wnd_);
    glfwWaitEvents();
  }
  glfwDestroyWindow(wnd_);
  glfwTerminate();

}

Size2i Window::framebuffer_size() const {
  int width = 0;
  int height = 0;
  glfwGetFramebufferSize(wnd_, &width, &height);
  return {width, height};
}

float Window::aspect_ratio() const {
  const auto size = framebuffer_size();
  return static_cast<float>(size.width) / static_cast<float>(size.height);
}

void Window::on_key_event(const KeyEvent&) {}

void Window::key_callback(GLFWwindow* wnd, const int key,
                          const int scancode, const int action,
                          const int mods) {
  Window* window = static_cast<Window*>(glfwGetWindowUserPointer(wnd));
  KeyEvent event{key, scancode, action, mods};
  window->on_key_event(event);
}

}  // namespace mgrid
