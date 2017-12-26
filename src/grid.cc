#include "grid.hh"

#include <memory>

#include <Qt3DRender/QAttribute>
#include <Qt3DRender/QBuffer>
#include <Qt3DRender/QCullFace>
#include <Qt3DRender/QEffect>
#include <Qt3DRender/QGeometry>
#include <Qt3DRender/QGraphicsApiFilter>
#include <Qt3DRender/QMaterial>
#include <Qt3DRender/QRenderPass>
#include <Qt3DRender/QShaderProgram>
#include <Qt3DRender/QTechnique>
#include <QUrl>

#include <QVector3D>
#include <qmath.h>

using Qt3DRender::QCullFace;
using Qt3DRender::QEffect;
using Qt3DRender::QFilterKey;
using Qt3DRender::QGraphicsApiFilter;
using Qt3DRender::QMaterial;
using Qt3DRender::QRenderPass;
using Qt3DRender::QShaderProgram;
using Qt3DRender::QTechnique;

std::unique_ptr<QMaterial> create_grid_material() {
  std::unique_ptr<QMaterial> material{new QMaterial()};

  // Create effect, technique, render pass and shader
  QEffect *effect = new QEffect();
  QTechnique *technique = new QTechnique();
  QRenderPass *pass = new QRenderPass();
  QShaderProgram *prog = new QShaderProgram();

  // Set the shader on the render pass
  pass->setShaderProgram(prog);
  prog->setVertexShaderCode(
      QShaderProgram::loadSource(QUrl("qrc:///shaders/grid_vert.glsl")));
  prog->setTessellationControlShaderCode(
      QShaderProgram::loadSource(QUrl("qrc:///shaders/grid_tess_ctrl.glsl")));
  prog->setTessellationEvaluationShaderCode(
      QShaderProgram::loadSource(QUrl("qrc:///shaders/grid_tess_eval.glsl")));
  prog->setGeometryShaderCode(
      QShaderProgram::loadSource(QUrl("qrc:///shaders/grid_geom.glsl")));
  prog->setFragmentShaderCode(
      QShaderProgram::loadSource(QUrl("qrc:///shaders/grid_frag.glsl")));

  auto* cull = new QCullFace();
  cull->setMode(QCullFace::NoCulling);
  pass->addRenderState(cull);

  // Add the pass to the technique
  technique->addRenderPass(pass);

  auto* filter = new QFilterKey();
  filter->setName(QStringLiteral("renderingStyle"));
  filter->setValue(QStringLiteral("forward"));

  technique->addFilterKey(filter);

  // Set the targeted GL version for the technique
  technique->graphicsApiFilter()->setApi(QGraphicsApiFilter::OpenGL);
  technique->graphicsApiFilter()->setMajorVersion(4);
  technique->graphicsApiFilter()->setMinorVersion(1);
  technique->graphicsApiFilter()->setProfile(QGraphicsApiFilter::CoreProfile);

  effect->addTechnique(technique);
  material->setEffect(effect);

  QObject::connect(prog, &QShaderProgram::logChanged, [prog]() {
      qInfo() << prog->log();
    });

  return material;
}

class GridGeometry : public Qt3DRender::QGeometry {
public:
  explicit GridGeometry(QNode *parent = nullptr);
  ~GridGeometry() override {}

private:
  void createVertexData();

  Qt3DRender::QAttribute *m_positionAttribute;
  Qt3DRender::QBuffer *m_vertexBuffer;
};

GridGeometry::GridGeometry(QNode *parent) : QGeometry(parent) {

  m_positionAttribute = new Qt3DRender::QAttribute(this);

  m_vertexBuffer =
      new Qt3DRender::QBuffer(Qt3DRender::QBuffer::VertexBuffer, this);

  // vec3 pos
  const quint32 elementSize = 3;
  const quint32 stride = elementSize * sizeof(float);
  const int nVerts = 4;

  m_positionAttribute->setName(
      Qt3DRender::QAttribute::defaultPositionAttributeName());
  m_positionAttribute->setDataType(Qt3DRender::QAttribute::Float);
  m_positionAttribute->setDataSize(3);
  m_positionAttribute->setAttributeType(
      Qt3DRender::QAttribute::VertexAttribute);
  m_positionAttribute->setBuffer(m_vertexBuffer);
  m_positionAttribute->setByteStride(stride);
  m_positionAttribute->setCount(nVerts);

  createVertexData();

  addAttribute(m_positionAttribute);
}

void GridGeometry::createVertexData() {
  const int verticesCount = 4;
  // vec3 pos, vec3 normal
  const quint32 vertexSize = 3 * sizeof(float);

  QByteArray verticesData;
  verticesData.resize(vertexSize * verticesCount);
  float *f = reinterpret_cast<float *>(verticesData.data());

  const float size = 10;

  f[0] = -size;
  f[1] = -size;
  f[2] = 0;

  f[3] = size;
  f[4] = -size;
  f[5] = 0;

  f[6] = size;
  f[7] = size;
  f[8] = 0;

  f[9] = -size;
  f[10] = size;
  f[11] = 0;

  m_vertexBuffer->setData(verticesData);
}

GridRenderer::GridRenderer(Qt3DCore::QNode *parent)
    : QGeometryRenderer(parent) {
  setGeometry(new GridGeometry(this));
  setVerticesPerPatch(4);
  setVertexCount(4);
  setPrimitiveType(Patches);
}

GridRenderer::~GridRenderer() {}
