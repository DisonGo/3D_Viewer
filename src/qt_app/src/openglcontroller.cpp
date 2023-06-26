#include "openglcontroller.h"

#include <QFileInfo>
#include <QMouseEvent>

void OpenGLController::mousePressEvent(QMouseEvent *e) {
  if (e->button() == Qt::LeftButton) {
    LMB_pressed = true;
    mPos = e->pos();
    cameraSpacer->mousePressSlot(e);
  }
}

void OpenGLController::mouseMoveEvent(QMouseEvent *e) {
  if (!LMB_pressed) return;
  cameraSpacer->mouseMoveSlot(e);
  update();
}

void OpenGLController::mouseReleaseEvent(QMouseEvent *e) {
  if (e->button() == Qt::LeftButton) {
    LMB_pressed = false;
    mPos = e->pos();
    if (cameraSpacer) cameraSpacer->mouseReleaseSlot(e);
  }
}

void OpenGLController::keyPressEvent(QKeyEvent *e) {
  if (cameraSpacer) cameraSpacer->keyPressSlot(e);
  update();
}
void OpenGLController::keyReleaseEvent(QKeyEvent *e) {
  if (cameraSpacer) cameraSpacer->keyReleaseSlot(e);
  update();
}
void OpenGLController::initializeGL() {
  initializeOpenGLFunctions();
  QSize winSize = this->size();
  calcSizes(winSize.width(), winSize.height());
  glClearColor(0, 0, 0, 1);
  glEnable(GL_PROGRAM_POINT_SIZE);
  glEnable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_LINE_SMOOTH);
  glEnable(GL_MULTISAMPLE);
  glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
  engine = new Engine();
  cameraSpacer = new CameraSpacer(this, *engine->GetCurrentCamera());
  emit initialized();
}

void OpenGLController::resizeGL(int w, int h) {
  glViewport(0, 0, w, h);
  calcSizes(w, h);
  cameraSpacer->SetVw(vw);
  cameraSpacer->SetVh(vh);
  update();
}
void OpenGLController::paintGL() {
  glClearColor(BackColor.redF(), BackColor.greenF(), BackColor.blueF(), 1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  if (drawArrConf.Points) {
    engine->drawGeometry(GL_POINTS);
  }
  if (drawArrConf.Triangles) {
    engine->drawGeometry(GL_TRIANGLES);
  }
  if (drawArrConf.Lines) {
    engine->drawGeometry(GL_LINES);
  }
}
void OpenGLController::calcSizes(int w, int h) {
  vw = w;
  vh = h;
  ratio = vw / vh;
}

void OpenGLController::setDrawArrConfig(struct glDrawArraysConfig config) {
  drawArrConf = config;
  update();
}
std::vector<QImage> OpenGLController::getScreencast() {
  std::vector<QImage> gifFrames;
  int totalFrames = gifFps * gifLength;

  float animationAnglePerSecond = 360 / totalFrames;
  float animationCurentAngle = rotationVec.y();
  float rememberAngle = animationCurentAngle;
  for (int currentFrame = 0; currentFrame < totalFrames; currentFrame++) {
    QImage frame = grabFramebuffer();
    frame = frame.scaled(gifResolution);
    gifFrames.push_back(frame);
    animationCurentAngle += animationAnglePerSecond;
    rotationVec.setY(animationCurentAngle);
    update();
  }
  rotationVec.setY(rememberAngle);
  update();
  return gifFrames;
}
void OpenGLController::capture() {
  QImage frame = grabFramebuffer();
  frame = frame.scaled(gifResolution);
  captureBuffer.push_back(frame);
}
void OpenGLController::startScreenCapture(int FPS) {
  connect(&captureTimer, SIGNAL(timeout()), this, SLOT(capture()));
  captureTimer.start(1000 / FPS);
}
std::vector<QImage> OpenGLController::stopScreenCapture() {
  captureTimer.stop();
  return captureBuffer;
}
std::vector<Transform *> OpenGLController::GetMeshTransforms() {
  if (!engine) return std::vector<Transform *>();
  return engine->GetMeshTransforms();
}

std::vector<Mesh *> OpenGLController::GetMeshes() {
  if (!engine) return std::vector<Mesh *>();
  return engine->GetMeshes();
}
void OpenGLController::importObjFile(QString filename) {
  if (!engine) return;
  makeCurrent();
  engine->importObj(filename);
  QFileInfo fileInfo(filename);
  update();
  emit importComleted(engine->verticesN, engine->verticesN / 3 * 2,
                      fileInfo.fileName());
}
OpenGLController::~OpenGLController() {
  delete engine;
  delete program;
  delete cameraSpacer;
}
