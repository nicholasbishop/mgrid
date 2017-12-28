#ifndef SRC_SCULPT_HH_
#define SRC_SCULPT_HH_

#include "common.hh"

namespace mgrid {

class Grid;

class Sculpt {
 public:
  explicit Sculpt(Grid& grid);
  ~Sculpt();

  void start();
  void move(const vec3& center);
  void stop();

  bool in_drag() const { return in_drag_; }

 private:
  Grid& grid_;
  float radius_ = 0.2f;
  float strength_ = 0.2f;
  bool add_ = true;
  bool in_drag_ = false;
};

}  // namespace mgrid

#endif  // SRC_SCULPT_HH_
