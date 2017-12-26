#ifndef SRC_GRID_TEXTURE_HH_
#define SRC_GRID_TEXTURE_HH_

#include <vector>

#include <Qt3DRender/QAbstractTexture>

class GridTexture : public Qt3DRender::QAbstractTexture {
 public:
  void update();

 private:
  std::vector<float> points_;
};

#endif  // SRC_GRID_TEXTURE_HH_
