#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#include <epoxy/gl.h>

#include "cmrc/cmrc.hpp"

#include "camera.hh"
#include "camera_controller.hh"
#include "common.hh"
#include "draw_mesh.hh"
#include "draw_ray.hh"
#include "errors.hh"
#include "grid.hh"
#include "resources.hh"
#include "sculpt.hh"
#include "shader.hh"
#include "texture.hh"
#include "vao.hh"
#include "window.hh"

using namespace mgrid;

class Axes {
 public:
  Axes() {
    mesh_ = DrawMesh();
    mesh_.vao().bind();
    auto& vbo = *mesh_.add_array_buffer();

    auto& program = mesh_.program();
    program.create_vert_shader(
        read_resource_string("src/shaders/basic.vert.glsl"));
    program.create_frag_shader(
        read_resource_string("src/shaders/basic.frag.glsl"));
    program.link();

    vec3 vertices[2 * 3] = {
        {0, 0, 0}, {1, 0, 0}, {0, 0, 0}, {0, 1, 0}, {0, 0, 0}, {0, 0, 1},
    };
    vbo.set_data(vertices, sizeof(vertices), GL_STATIC_DRAW);
    const auto vpos_location = program.attribute_location("vPos");
    mesh_.vao().set_attribute_data(vpos_location, 3, GL_FLOAT, nullptr);
  }

  void draw(const mat4& mvp) {
    mesh_.program().set_uniform("MVP", mvp);
    mesh_.draw(GL_LINES, 6);
  }

 private:
  DrawMesh mesh_;
};

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
    if (camera_controller_.in_rotate()) {
      camera_controller_.set_rotate(event.pos);
    } else if (sculpt_.in_drag()) {
      const auto ray = camera_.ray(event.pos);
      const auto hit = grid_.intersect_ray(ray);
      if (hit) {
        const auto hit_loc = ray.origin + ray.direction * hit->distance;
        sculpt_.move(hit_loc);
        update_grid_texture();
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
        mouse_draw_ray_->update_ray(camera_.ray(event.pos));

        if (over_mesh(event.pos)) {
          sculpt_.start();
        } else {
          camera_controller_.start_rotate(event.pos);
        }
      } else {
        camera_controller_.end_rotate();
        sculpt_.stop();
      }
    }
  }

  void init_grid_mesh() {
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

    grid_draw_mesh_ = DrawMesh();
    auto* vbo = grid_draw_mesh_->add_array_buffer();
    vbo->set_data(vertices, sizeof(vertices), GL_STATIC_DRAW);

    auto& program = grid_draw_mesh_->program();
    program.create_vert_shader(vs);
    program.create_frag_shader(fs);
    program.create_geom_shader(gs);
    program.create_tess_ctrl_shader(tcs);
    program.create_tess_eval_shader(tes);

    program.link();

    const auto vpos_location = program.attribute_location("vertexPosition");
    grid_draw_mesh_->vao().set_attribute_data(vpos_location, 2, GL_FLOAT,
                                              nullptr);

    grid_.make_random({64, 64});

    grid_texture_ = grid_draw_mesh_->add_texture();
    update_grid_texture();

    const auto tex_location = program.uniform_location("gridTex");
    glUniform1i(tex_location, 0);
    // glActiveTexture(GL_TEXTURE0 + 0);
    // grid_texture.bind();
  }

  void update_grid_texture() {
    grid_texture_->set_data(GL_RGB32F, grid_.res(), GL_RGB, GL_FLOAT,
                            grid_.data());
  }

  void initialize() final {
    init_grid_mesh();
    axes_ = Axes();
    mouse_draw_ray_ = DrawRay();
  }

  void render() final {
    const auto size = framebuffer_size();
    camera_.set_size(size);
    glViewport(0, 0, size.x, size.y);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    const auto mvp = camera_.view_projection_matrix();
    grid_draw_mesh_->program().bind();
    grid_draw_mesh_->program().set_uniform("modelViewProjection", mvp);
    glPatchParameteri(GL_PATCH_VERTICES, 4);
    grid_draw_mesh_->draw(GL_PATCHES, 4);

    axes_->draw(mvp);
    mouse_draw_ray_->draw(mvp);
  }

  void clean_up() {
    grid_draw_mesh_ = nullopt;
    axes_ = nullopt;
    mouse_draw_ray_ = nullopt;
  }

  optional<DrawMesh> grid_draw_mesh_;
  optional<Axes> axes_;
  optional<DrawRay> mouse_draw_ray_;

  Camera camera_;
  CameraController camera_controller_{camera_};
  Grid grid_;
  Sculpt sculpt_{grid_};
  Texture* grid_texture_;
};

int main(void) {
  CMRC_INIT(resources);

  App app;
  app.start();

  return 0;
}
