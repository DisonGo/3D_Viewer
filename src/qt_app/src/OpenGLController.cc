#include "OpenGLController.h"

#include <QFileInfo>
#include <QMouseEvent>
#include <QThread>
#include <QTimer>
void OpenGLController::mousePressEvent(QMouseEvent *e) {
  if (e->button() == Qt::LeftButton) {
    LMB_pressed = true;
    if (!camera_spacer_) return;
    camera_spacer_->mousePressSlot(e);
  }
}

void OpenGLController::mouseMoveEvent(QMouseEvent *e) {
  if (!LMB_pressed || !camera_spacer_) return;
  camera_spacer_->mouseMoveSlot(e);
}

void OpenGLController::mouseReleaseEvent(QMouseEvent *e) {
  if (e->button() == Qt::LeftButton) {
    LMB_pressed = false;
    if (camera_spacer_) camera_spacer_->mouseReleaseSlot(e);
  }
}

void OpenGLController::keyPressEvent(QKeyEvent *e) {
  if (camera_spacer_) camera_spacer_->keyPressSlot(e);
}
void OpenGLController::keyReleaseEvent(QKeyEvent *e) {
  if (camera_spacer_) camera_spacer_->keyReleaseSlot(e);
}

void OpenGLController::wheelEvent(QWheelEvent *e) {
  if (camera_spacer_) camera_spacer_->wheelEventSlot(e);
}
void OpenGLController::initializeGL() {
  setAutoFillBackground(false);
  initializeOpenGLFunctions();
  makeCurrent();
  QSize winSize = this->size();
  CalcSizes(winSize.width(), winSize.height());
  engine_spacer_.InitializeEngine();
  auto cam = engine_spacer_.GetCurrentCamera();
  if (cam) {
    cam->SetVh(vh);
    cam->SetVw(vw);
  }
  emit Initialized();
}

void OpenGLController::resizeGL(int w, int h) {
  glViewport(0, 0, w, h);
  CalcSizes(w, h);
  update();
}
void OpenGLController::paintGL() {
  makeCurrent();
  engine_spacer_.RequestRenderCycle();
  doneCurrent();
}
void OpenGLController::CalcSizes(int w, int h) {
  vw = w;
  vh = h;
  if (camera_spacer_) camera_spacer_->SetVh(vh);
  if (camera_spacer_) camera_spacer_->SetVw(vw);
  ratio = vw / vh;
}
void OpenGLController::CaptureBuffer() {
  QImage frame = grabFramebuffer().scaled(gifResolution);
  captureBuffer.push_back(frame);
}

void OpenGLController::StartScreenCapture(int FPS) {
  connect(&captureTimer, SIGNAL(timeout()), this, SLOT(capture()));
  captureTimer.start(1000 / FPS);
}
std::vector<QImage> OpenGLController::StopScreenCapture() {
  captureTimer.stop();
  return captureBuffer;
}

void OpenGLController::SetCameraSpacer(s21::CameraSpacer *spacer) {
  camera_spacer_ = spacer;
  if (!camera_spacer_) return;
  engine_spacer_.SetCurrentCamera(camera_spacer_->GetCamera());
  camera_spacer_->SetVh(vh);
  camera_spacer_->SetVw(vw);
  update();
}
OpenGLController::~OpenGLController() {}
