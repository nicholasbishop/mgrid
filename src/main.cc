#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#include <epoxy/gl.h>

#include "cmrc/cmrc.hpp"

#include "camera.hh"
#include "common.hh"
#include "errors.hh"
#include "grid.hh"
#include "shader.hh"
#include "texture.hh"
#include "window.hh"

using namespace mgrid;

std::string read_resource_string(const std::string& path) {
  auto r = cmrc::open(path);
  if (!r.begin()) {
    throw std::runtime_error("invalid resource: " + path);
  }
  return {r.begin(), r.end()};
}

class App : public Window {
 public:
  App() : Window(GLVersion(4, 0)) {}

 private:
  bool over_mesh(const dvec2& pos) {
    const auto ray = camera_.ray(pos);
    const auto hit = grid_.intersect_ray(ray);
    return !!hit;
  }

  void on_cursor_position_event(const CursorPositionEvent& event) final {
    if (in_left_drag_) {
      if (over_mesh(event.pos)) {
        std::cout << "over mesh" << std::endl;
      } else {
        std::cout << "bg" << std::endl;
      }
    }
  }

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

  void on_mouse_button_event(const MouseButtonEvent& event) final {
    if (event.isLeftButton()) {
      if (event.isPress()) {
        in_left_drag_ = true;
        left_drag_start_ = event.pos;
      } else {
        in_left_drag_ = false;
      }
    }
  }

  void initialize() final {
    vec2 vertices[4] = {
        {-1.0f, -1.0f}, {1.0f, -1.0f}, {1.0f, 1.0f}, {-1.0f, 1.0f},
    };

    const auto vs = read_resource_string("src/shaders/grid_vert.glsl");
    const auto fs = read_resource_string("src/shaders/grid_frag.glsl");
    const auto gs = read_resource_string("src/shaders/grid_geom.glsl");
    const auto tcs = read_resource_string("src/shaders/grid_tess_ctrl.glsl");
    const auto tes = read_resource_string("src/shaders/grid_tess_eval.glsl");

    glDisable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);

    // NOTE: OpenGL error checks have been omitted for brevity
    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    program_ = ShaderProgram();
    program_->create_vert_shader(vs);
    program_->create_frag_shader(fs);
    program_->create_geom_shader(gs);
    program_->create_tess_ctrl_shader(tcs);
    program_->create_tess_eval_shader(tes);

    program_->link();

    mvp_location = program_->uniform_location("modelViewProjection");
    vpos_location = program_->attribute_location("vertexPosition");
    glEnableVertexAttribArray(vpos_location);
    const int stride = 0;
    glVertexAttribPointer(vpos_location, 2, GL_FLOAT, GL_FALSE, stride,
                          nullptr);

    grid_.make_random({4, 4});

    grid_texture_ = Texture(GL_TEXTURE_2D);
    grid_texture_->bind();
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, grid_.res().x, grid_.res().y,
                 0, GL_RGB, GL_FLOAT, grid_.data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    const auto tex_location = program_->uniform_location("gridTex");
    glUniform1i(tex_location, 0);
    glActiveTexture(GL_TEXTURE0 + 0);
    grid_texture_->bind();
  }

  void render() final {
    const auto size = framebuffer_size();
    camera_.set_size(size);
    glViewport(0, 0, size.x, size.y);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    const auto mvp = camera_.view_projection_matrix();
    program_->bind();
    glUniformMatrix4fv(mvp_location, 1, GL_FALSE, &mvp[0][0]);
    glPatchParameteri(GL_PATCH_VERTICES, 4);
    glDrawArrays(GL_PATCHES, 0, 4);
  }

  void clean_up() {
    program_ = nullopt;
    grid_texture_ = nullopt;
  }

  optional<ShaderProgram> program_;
  optional<Texture> grid_texture_;
  GLuint vao_;
  GLuint vertex_buffer;
  GLint mvp_location, vpos_location;
  Camera camera_;
  bool in_left_drag_ = false;
  vec2 left_drag_start_;
  Grid grid_;
};

int main(void) {
  CMRC_INIT(resources);

  App app;
  app.start();

  return 0;
}
