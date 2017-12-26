#ifndef SRC_GRID_HH_
#define SRC_GRID_HH_

#include <Qt3DRender/QGeometryRenderer>

class GridRenderer : public Qt3DRender::QGeometryRenderer {
public:
  explicit GridRenderer(Qt3DCore::QNode *parent = nullptr);
  ~GridRenderer() override;
};

#endif // SRC_GRID_HH_
