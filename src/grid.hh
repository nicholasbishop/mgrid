#ifndef SRC_GRID_HH_
#define SRC_GRID_HH_

#include <memory>

#include <Qt3DRender/QEffect>
#include <Qt3DRender/QGeometryRenderer>
#include <Qt3DRender/QMaterial>
#include <Qt3DRender/QParameter>
#include <Qt3DRender/QRenderPass>
#include <Qt3DRender/QShaderProgram>
#include <Qt3DRender/QTechnique>
#include <Qt3DRender/QTexture>

class ImageDataGen : public Qt3DRender::QTextureImageDataGenerator {
 public:
  ImageDataGen();

  Qt3DRender::QTextureImageDataPtr	operator()() final {
    return data_;
  }

 private:
  QSharedPointer<Qt3DRender::QTextureImageData> data_;
};

class GridTextureImage : public Qt3DRender::QAbstractTextureImage {
  Q_OBJECT

 public:
  GridTextureImage();

 protected:
  Qt3DRender::QTextureImageDataGeneratorPtr dataGenerator() const final;

 private:
  QSharedPointer<ImageDataGen> gen_;
};

class GridRenderer : public Qt3DRender::QGeometryRenderer {
public:
  explicit GridRenderer(Qt3DCore::QNode *parent = nullptr);
  ~GridRenderer() override;

 private:
  GridTextureImage gridTexImage_;
};

class GridMaterial : public Qt3DRender::QMaterial {
public:
  GridMaterial();

private:
  Qt3DRender::QEffect effect_;
  Qt3DRender::QTechnique technique_;
  Qt3DRender::QRenderPass pass_;
  Qt3DRender::QShaderProgram prog_;
  Qt3DRender::QParameter texParam_;
  Qt3DRender::QTextureLoader texLoader_;
  // Qt3DRender::QTexture2D gridTex_;
  // Qt3DRender::QTextureImage placeholderTexImage_;
};

#endif // SRC_GRID_HH_
