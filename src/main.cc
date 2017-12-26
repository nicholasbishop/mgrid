#include <QCamera>
#include <QGuiApplication>
#include <QOrbitCameraController>
#include <QPhongMaterial>
#include <QRenderSettings>
#include <QTorusMesh>
#include <QTransform>
#include <Qt3DWindow>

#include "src/mesh.hh"

Mesh MakeCube() {
  Mesh mesh;
  
  return mesh;
}

Qt3DCore::QEntity *createScene() {
  Qt3DCore::QEntity *rootEntity = new Qt3DCore::QEntity;

  Qt3DRender::QMaterial *material = new Qt3DExtras::QPhongMaterial(rootEntity);

  Qt3DCore::QEntity *torusEntity = new Qt3DCore::QEntity(rootEntity);
  Qt3DExtras::QTorusMesh *torusMesh = new Qt3DExtras::QTorusMesh;
  torusMesh->setRadius(5);
  torusMesh->setMinorRadius(1);
  torusMesh->setRings(100);
  torusMesh->setSlices(20);

  torusEntity->addComponent(torusMesh);
  torusEntity->addComponent(material);

  return rootEntity;
}

int main(int argc, char *argv[]) {
  QGuiApplication app(argc, argv);
  Qt3DExtras::Qt3DWindow view;
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
