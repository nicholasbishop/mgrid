// Copyright 2017 Neverware

#ifndef SRC_DRAW_RAY_HH_
#define SRC_DRAW_RAY_HH_

#include "common.hh"
#include "draw_mesh.hh"

namespace mgrid {

class Ray3;

class DrawRay {
 public:
  DrawRay();

  void update_ray(const Ray3& ray);

  void draw(const mat4& mvp);

 private:
  DrawMesh mesh_;
  Vbo* vbo_;
};

}  // mgrid

#endif  // SRC_DRAW_RAY_HH_
