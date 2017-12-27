#include <epoxy/gl.h>

#include "camera.hh"
#include "common.hh"
#include "window.hh"

using namespace mgrid;

static const char *vertex_shader_text =
    "uniform mat4 MVP;\n"
    "attribute vec3 vCol;\n"
    "attribute vec2 vPos;\n"
    "varying vec3 color;\n"
    "void main()\n"
    "{\n"
    "    gl_Position = MVP*  vec4(vPos, 0.0, 1.0);\n"
    "    color = vCol;\n"
    "}\n";

static const char *fragment_shader_text =
    "varying vec3 color;\n"
    "void main()\n"
    "{\n"
    "    gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);\n"
    "}\n";


class App : public Window {
 public:
  App() : Window(GLVersion(2, 0)) {}

 private:
  void on_key_event(const KeyEvent& event) final {
    if (!event.isPress()) {
      return;
    }

    const Angle angle_delta = Angle::from_degrees(5);

    if (event.isEscape()) {
      close();
    } else if (event.isLeftArrow()) {
      camera_.set_around_angle(camera_.around_angle() - angle_delta);
    } else if (event.isRightArrow()) {
      camera_.set_around_angle(camera_.around_angle() + angle_delta);
    } else if (event.isUpArrow()) {
      camera_.set_height_angle(camera_.height_angle() + angle_delta);
    } else if (event.isDownArrow()) {
      camera_.set_height_angle(camera_.height_angle() - angle_delta);
    }
  }

  void initialize() final {
    vec2 vertices[4] = {
      {-1.0f, -1.0f},
      { 1.0f, -1.0f},
      { 1.0f,  1.0f},
      {-1.0f,  1.0f},
    };

    // NOTE: OpenGL error checks have been omitted for brevity
    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_text, NULL);
    glCompileShader(vertex_shader);
    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_text, NULL);
    glCompileShader(fragment_shader);
    program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);
    mvp_location = glGetUniformLocation(program, "MVP");
    vpos_location = glGetAttribLocation(program, "vPos");
    glEnableVertexAttribArray(vpos_location);
    const int stride = 0;
    glVertexAttribPointer(vpos_location, 2, GL_FLOAT, GL_FALSE, stride,
                          (void *)0);
  }

  void render() final {
    const auto size = framebuffer_size();
    camera_.set_size(size.width, size.height);
    glViewport(0, 0, size.width, size.height);
    glClear(GL_COLOR_BUFFER_BIT);
    const auto mvp = camera_.view_projection_matrix();
    glUseProgram(program);
    glUniformMatrix4fv(mvp_location, 1, GL_FALSE, &mvp[0][0]);
    glDrawArrays(GL_LINE_LOOP, 0, 4);
  }

  GLuint vertex_buffer, vertex_shader, fragment_shader, program;
  GLint mvp_location, vpos_location;
  Camera camera_;
};

int main(void) {
  App app;
  app.start();

  return 0;
}
