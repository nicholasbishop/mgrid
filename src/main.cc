#include <QAttribute>
#include <QBuffer>
#include <QCamera>
#include <QDirectionalLight>
#include <QGeometry>
#include <QGeometryRenderer>
#include <QGuiApplication>
#include <QObjectPicker>
#include <QOrbitCameraController>
#include <QPhongMaterial>
#include <QPickEvent>
#include <QRenderSettings>
#include <QTorusMesh>
#include <QTransform>
#include <Qt3DWindow>

#include "src/grid.hh"
#include "src/mesh.hh"

using Qt3DCore::QEntity;
using namespace Qt3DRender;

Grid makeBumpyGrid(int w, int h) {
  Grid grid;
  grid.width = w;
  grid.height = h;
  grid.points.reserve(grid.width * grid.height);
  for (int y = 0; y < grid.height; y++) {
    for (int x = 0; x < grid.width; x++) {
      const float z = (rand() % 128) / 128.0f;
      grid.points.emplace_back(x, y, z);
    }
  }
  return grid;
}

Qt3DCore::QEntity *createScene() {
  Qt3DCore::QEntity *rootEntity = new Qt3DCore::QEntity;

  auto *material = new Qt3DExtras::QPhongMaterial(rootEntity);
  material->setDiffuse(QColor(204, 205, 255));

  Qt3DCore::QEntity *torusEntity = new Qt3DCore::QEntity(rootEntity);
  Qt3DExtras::QTorusMesh *torusMesh = new Qt3DExtras::QTorusMesh;
  torusMesh->setRadius(5);
  torusMesh->setMinorRadius(1);
  torusMesh->setRings(100);
  torusMesh->setSlices(20);

  torusEntity->addComponent(torusMesh);
  torusEntity->addComponent(material);

  auto *bumpyEntity = new QEntity(rootEntity);
  auto *bumpyGeom = new Qt3DRender::QGeometry();
  Grid grid = makeBumpyGrid(64, 64);
  auto *bumpyBuf = new QBuffer();
  bumpyBuf->setData(QByteArray((char *)grid.points.data(),
                               grid.points.size() * sizeof(QVector3D)));
  auto *bumpyAttr = new QAttribute(
      bumpyBuf, QAttribute::defaultPositionAttributeName(), QAttribute::Float,
      sizeof(float) * 3, grid.width * grid.height, 0, 0);
  bumpyAttr->setAttributeType(QAttribute::VertexAttribute);
  bumpyGeom->addAttribute(bumpyAttr);
  auto *bumpyMesh = new QGeometryRenderer(bumpyGeom);
  bumpyMesh->setVertexCount(grid.width * grid.height);
  bumpyMesh->setPrimitiveType(QGeometryRenderer::Points);
  bumpyEntity->addComponent(bumpyMesh);
  bumpyEntity->addComponent(material);

  auto *gridEntity = new QEntity(rootEntity);
  auto *gridRenderer = new GridRenderer();
  gridEntity->addComponent(gridRenderer);
  auto *gridMaterial = new GridMaterial();
  gridEntity->addComponent(gridMaterial);
  // gridEntity->addComponent(material);

  auto *light = new Qt3DRender::QDirectionalLight();
  light->setWorldDirection({1, 1, -1});
  rootEntity->addComponent(light);

  return rootEntity;
}

int main(int argc, char *argv[]) {
  QGuiApplication app(argc, argv);
  Qt3DExtras::Qt3DWindow view;
  view.setWidth(640);
  view.setHeight(480);
  view.renderSettings()->setRenderPolicy(Qt3DRender::QRenderSettings::OnDemand);

  Qt3DCore::QEntity *scene = createScene();

  // Camera
  Qt3DRender::QCamera *camera = view.camera();
  camera->lens()->setPerspectiveProjection(45.0f, 16.0f / 9.0f, 0.1f, 1000.0f);
  camera->setPosition(QVector3D(0, 0, 40.0f));
  camera->setViewCenter(QVector3D(0, 0, 0));

  // For camera controls
  Qt3DExtras::QOrbitCameraController *camController =
      new Qt3DExtras::QOrbitCameraController(scene);
  camController->setLinearSpeed(50.0f);
  camController->setLookSpeed(180.0f);
  camController->setCamera(camera);

  view.setRootEntity(scene);
  view.show();

  return app.exec();
}
