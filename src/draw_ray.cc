#include "draw_ray.hh"

#include "draw_mesh.hh"
#include "ray.hh"
#include "resources.hh"

namespace mgrid {

DrawRay::DrawRay() {
  mesh_ = DrawMesh();
  mesh_.vao().bind();
  vbo_ = mesh_.add_array_buffer();

  auto& program = mesh_.program();
  program.create_vert_shader(
      read_resource_string("src/shaders/basic.vert.glsl"));
  program.create_frag_shader(
      read_resource_string("src/shaders/basic.frag.glsl"));
  program.link();

  const auto vpos_location = program.attribute_location("vPos");
  mesh_.vao().set_attribute_data(vpos_location, 3, GL_FLOAT, nullptr);
}

void DrawRay::update_ray(const Ray3& ray) {
  vec3 vertices[2] = {ray.origin, ray.origin + ray.direction};
  vbo_->set_data(vertices, sizeof(vertices), GL_STATIC_DRAW);
}

void DrawRay::draw(const mat4& mvp) {
  mesh_.program().set_uniform("MVP", mvp);
  mesh_.draw(GL_LINES, 2);
}

}  // namespace mgrid
