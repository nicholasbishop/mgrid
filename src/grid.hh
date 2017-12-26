#ifndef SRC_GRID_HH_
#define SRC_GRID_HH_

#include <memory>

#include <Qt3DRender/QGeometryRenderer>
#include <Qt3DRender/QMaterial>

std::unique_ptr<Qt3DRender::QMaterial> create_grid_material();

class GridRenderer : public Qt3DRender::QGeometryRenderer {
public:
  explicit GridRenderer(Qt3DCore::QNode *parent = nullptr);
  ~GridRenderer() override;
};

#endif // SRC_GRID_HH_
