#include "grid.hh"

#include <Qt3DRender/QAttribute>
#include <Qt3DRender/QBuffer>
#include <Qt3DRender/QGeometry>

#include <QVector3D>
#include <qmath.h>

class GridGeometry : public Qt3DRender::QGeometry {
public:
  explicit GridGeometry(QNode *parent = nullptr);
  ~GridGeometry() override {}

private:
  void createVertexData();
  void createIndexData();

  Qt3DRender::QAttribute *m_positionAttribute;
  Qt3DRender::QAttribute *m_normalAttribute;
  Qt3DRender::QAttribute *m_indexAttribute;

  Qt3DRender::QBuffer *m_vertexBuffer;
  Qt3DRender::QBuffer *m_indexBuffer;
};

GridGeometry::GridGeometry(QNode *parent) : QGeometry(parent) {
  m_positionAttribute = new Qt3DRender::QAttribute(this);
  m_normalAttribute = new Qt3DRender::QAttribute(this);
  m_indexAttribute = new Qt3DRender::QAttribute(this);

  m_vertexBuffer =
      new Qt3DRender::QBuffer(Qt3DRender::QBuffer::VertexBuffer, this);
  m_indexBuffer =
      new Qt3DRender::QBuffer(Qt3DRender::QBuffer::IndexBuffer, this);

  // vec3 pos, vec3 normal
  const quint32 elementSize = 3 + 3;
  const quint32 stride = elementSize * sizeof(float);
  const int nVerts = 4;
  const int faces = 2;

  m_positionAttribute->setName(
      Qt3DRender::QAttribute::defaultPositionAttributeName());
  m_positionAttribute->setDataType(Qt3DRender::QAttribute::Float);
  m_positionAttribute->setDataSize(3);
  m_positionAttribute->setAttributeType(
      Qt3DRender::QAttribute::VertexAttribute);
  m_positionAttribute->setBuffer(m_vertexBuffer);
  m_positionAttribute->setByteStride(stride);
  m_positionAttribute->setCount(nVerts);

  m_normalAttribute->setName(
      Qt3DRender::QAttribute::defaultNormalAttributeName());
  m_normalAttribute->setDataType(Qt3DRender::QAttribute::Float);
  m_normalAttribute->setDataSize(3);
  m_normalAttribute->setAttributeType(Qt3DRender::QAttribute::VertexAttribute);
  m_normalAttribute->setBuffer(m_vertexBuffer);
  m_normalAttribute->setByteStride(stride);
  m_normalAttribute->setByteOffset(3 * sizeof(float));
  m_normalAttribute->setCount(nVerts);

  m_indexAttribute->setAttributeType(Qt3DRender::QAttribute::IndexAttribute);
  m_indexAttribute->setDataType(Qt3DRender::QAttribute::UnsignedShort);
  m_indexAttribute->setBuffer(m_indexBuffer);

  m_indexAttribute->setCount(faces * 3);

  createVertexData();
  createIndexData();

  addAttribute(m_positionAttribute);
  addAttribute(m_normalAttribute);
  addAttribute(m_indexAttribute);
}

void GridGeometry::createVertexData() {
  const int verticesCount = 4;
  // vec3 pos, vec3 normal
  const quint32 vertexSize = (3 + 3) * sizeof(float);

  QByteArray verticesData;
  verticesData.resize(vertexSize * verticesCount);
  float *f = reinterpret_cast<float *>(verticesData.data());

  const float size = 10;

  f[0] = -size;
  f[1] = -size;
  f[2] = 0;
  f[3] = 0;
  f[4] = 0;
  f[5] = 1;

  f[6] = size;
  f[7] = -size;
  f[8] = 0;
  f[9] = 0;
  f[10] = 0;
  f[11] = 1;

  f[12] = size;
  f[13] = size;
  f[14] = 0;
  f[15] = 0;
  f[16] = 0;
  f[17] = 1;

  f[18] = -size;
  f[19] = size;
  f[20] = 0;
  f[21] = 0;
  f[22] = 0;
  f[23] = 1;

  m_vertexBuffer->setData(verticesData);
}

void GridGeometry::createIndexData() {
  const int facesCount = 2;
  const int indicesCount = facesCount * 3;
  const int indexSize = sizeof(quint16);
  Q_ASSERT(indicesCount < 65536);

  QByteArray indicesBytes;
  indicesBytes.resize(indicesCount * indexSize);
  quint16 *i = reinterpret_cast<quint16 *>(indicesBytes.data());

  i[0] = 0;
  i[1] = 1;
  i[2] = 2;

  i[3] = 0;
  i[4] = 2;
  i[5] = 3;

  m_indexBuffer->setData(indicesBytes);
}

GridRenderer::GridRenderer(Qt3DCore::QNode *parent)
    : QGeometryRenderer(parent) {
  setGeometry(new GridGeometry(this));
}

GridRenderer::~GridRenderer() {}
