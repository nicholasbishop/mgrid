#ifndef SRC_GRID_HH_
#define SRC_GRID_HH_

#include <memory>

#include <Qt3DRender/QEffect>
#include <Qt3DRender/QGeometryRenderer>
#include <Qt3DRender/QMaterial>
#include <Qt3DRender/QRenderPass>
#include <Qt3DRender/QShaderProgram>
#include <Qt3DRender/QTechnique>

class GridRenderer : public Qt3DRender::QGeometryRenderer {
public:
  explicit GridRenderer(Qt3DCore::QNode *parent = nullptr);
  ~GridRenderer() override;
};

class GridMaterial : public Qt3DRender::QMaterial {
 public:
  GridMaterial();

 private:
  Qt3DRender::QEffect effect_;
  Qt3DRender::QTechnique technique_;
  Qt3DRender::QRenderPass pass_;
  Qt3DRender::QShaderProgram prog_;
};

#endif // SRC_GRID_HH_
