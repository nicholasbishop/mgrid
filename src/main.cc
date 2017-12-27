#include <fstream>
#include <sstream>

#include <epoxy/gl.h>

#include "cmrc/cmrc.hpp"

#include "camera.hh"
#include "common.hh"
#include "errors.hh"
#include "shader.hh"
#include "window.hh"

using namespace mgrid;

std::string read_resource_string(const std::string& path) {
  auto r = cmrc::open(path);
  return {r.begin(), r.end()};
}

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

    const auto vs = read_resource_string("src/shaders/basic.vert.glsl");
    const auto fs = read_resource_string("src/shaders/basic.frag.glsl");

    // NOTE: OpenGL error checks have been omitted for brevity
    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    program_ = ShaderProgram();
    program_->create_vert_shader(vs);
    program_->create_frag_shader(fs);
    program_->link();

    mvp_location = program_->uniform_location("MVP");
    vpos_location = program_->attribute_location("vPos");
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
    program_->bind();
    glUniformMatrix4fv(mvp_location, 1, GL_FALSE, &mvp[0][0]);
    glDrawArrays(GL_LINE_LOOP, 0, 4);
  }

  void clean_up() {
    program_ = nullopt;
  }

  optional<ShaderProgram> program_;
  GLuint vertex_buffer;
  GLint mvp_location, vpos_location;
  Camera camera_;
};

int main(void) {
  CMRC_INIT(resources);

  App app;
  app.start();

  return 0;
}
