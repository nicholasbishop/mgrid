#include <epoxy/gl.h>

#include <GLFW/glfw3.h>

#include <iostream>

#include "errors.hh"
#include "window.hh"

namespace {

void error_callback(int error, const char* description) {
  fprintf(stderr, "Error %d: %s\n", error, description);
}

void gl_debug_callback(GLenum source,
                       GLenum type,
                       GLuint id,
                       GLenum severity,
                       GLsizei length,
                       const GLchar* message,
                       const void* UNUSED(userParam)) {
  const std::string msg{message, message + length};
  std::cerr << source << ":" << type << ":" << id << ":" << severity << ":"
            << msg << std::endl;
}

void init_glfw() {
  glfwSetErrorCallback(error_callback);
  if (!glfwInit())
    exit(EXIT_FAILURE);
}

static GLFWwindow* create_window(const mgrid::GLVersion& version) {
  init_glfw();

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, version.major);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, version.minor);
  GLFWwindow* window = glfwCreateWindow(1280, 800, "mgrid", NULL, NULL);
  if (!window) {
    glfwTerminate();
    exit(EXIT_FAILURE);
  }
  return window;
}
}

namespace mgrid {

GLVersion::GLVersion(const int major, const int minor)
    : major(major), minor(minor) {}

Window::Window(const GLVersion& version) : wnd_(create_window(version)) {
  glfwSetWindowUserPointer(wnd_, this);
  glfwSetKeyCallback(wnd_, &Window::key_callback);
  glfwSetCursorPosCallback(wnd_, &Window::cursor_pos_callback);
  glfwSetMouseButtonCallback(wnd_, &Window::mouse_button_callback);

  glfwMakeContextCurrent(wnd_);
  glfwSwapInterval(1);
}

void Window::close() {
  glfwSetWindowShouldClose(wnd_, true);
}

void Window::start() {
  glDebugMessageCallback(gl_debug_callback, nullptr);

  check_gl_error("pre-initialize");
  initialize();
  check_gl_error("post-initialize");

  while (!glfwWindowShouldClose(wnd_)) {
    check_gl_error("pre-render");
    render();
    check_gl_error("post-render");

    glfwSwapBuffers(wnd_);
    glfwWaitEvents();
  }

  clean_up();

  glfwDestroyWindow(wnd_);
  glfwTerminate();
}

ivec2 Window::framebuffer_size() const {
  int width = 0;
  int height = 0;
  glfwGetFramebufferSize(wnd_, &width, &height);
  return {width, height};
}

void Window::on_cursor_position_event(const CursorPositionEvent&) {}

void Window::on_key_event(const KeyEvent&) {}

void Window::on_mouse_button_event(const MouseButtonEvent&) {}

void Window::cursor_pos_callback(GLFWwindow* const wnd,
                                 const double xpos,
                                 const double ypos) {
  Window* window = from_user_pointer(wnd);
  CursorPositionEvent event{vec2{xpos, ypos}};
  window->on_cursor_position_event(event);
}

void Window::key_callback(GLFWwindow* wnd,
                          const int key,
                          const int scancode,
                          const int action,
                          const int mods) {
  Window* window = from_user_pointer(wnd);
  KeyEvent event{key, scancode, action, mods};
  window->on_key_event(event);
}

void Window::mouse_button_callback(GLFWwindow* const wnd,
                                   const int button,
                                   const int action,
                                   const int mods) {
  Window* window = from_user_pointer(wnd);
  dvec2 pos;
  glfwGetCursorPos(wnd, &pos.x, &pos.y);
  MouseButtonEvent event{button, action, mods, pos};
  window->on_mouse_button_event(event);
}

Window* Window::from_user_pointer(GLFWwindow* const wnd) {
  return static_cast<Window*>(glfwGetWindowUserPointer(wnd));
}

}  // namespace mgrid
