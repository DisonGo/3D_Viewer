#ifndef CAMERA_H
#define CAMERA_H
#include <QOpenGLFunctions>
#include <QObject>
#include <QVector>
#include "shader.h"
struct CameraData {
  QPoint centerPoint;
  QPoint relevantPoint;
  float rotX;
  float rotY;
  float angle;
  float speed;
  QVector3D orientation;
  QVector3D position;
};

class Camera : public QObject , protected QOpenGLFunctions
{
  Q_OBJECT
public:
  explicit Camera(QObject *parent = nullptr);
  // Stores the main vectors of the camera
  enum CameraMode {
    Free = 0,
    Focus = 1
  };
  // Camera constructor to set up initial values
  Camera(int width, int height, QVector3D position, QObject *parent = nullptr);
  // Updates and exports the camera matrix to the Vertex Shader
  void Matrix(float FOVdeg, float nearPlane, float farPlane, Shader& shader, const char* uniform);
  // Handles camera inputs
  //    void Inputs(GLFWwindow* window);
public slots:
  void keyPressSlot(QKeyEvent *e);
  void keyReleaseSlot(QKeyEvent *e);
  void mousePressSlot(QMouseEvent *e);
  void mouseReleaseSlot(QMouseEvent *e);
  void mouseMoveSlot(QMouseEvent *e);
  void wheelSlot(QWheelEvent *e);
public: // Setters / Getters
  const QVector3D &getPosition() const;
  void setPosition(const QVector3D &newPosition);
  int getVw() const;
  void setVw(int newVw);
  int getVh() const;
  void setVh(int newVh);
  void setSpeed(float newSpeed);
  CameraMode getMode() const;
  void setMode(CameraMode newMode);

  const QVector3D &getFocusPoint() const;

  void setFocusPoint(const QVector3D &newFocusPoint);

private: // Methods
  void processFreeMode(QPoint ePos);
  void processFocusMode(QPoint focusPoint);
private: // vars

  CameraMode mode = Free;

  QVector3D FocusPoint = QVector3D(0.0f, 0.0f, 0.0f);
  QVector3D Position = QVector3D(0,0,-100);
  QVector3D Orientation = QVector3D(0.0f, 0.0f, -1.0f);
  QVector3D Up = QVector3D(0.0f, 1.0f, 0.0f);

  bool LMBPressed = false;
  // Stores the width and height of the window
  int vw = 1;
  int vh = 1;
  QPoint mCenterPos = QPoint(0,0);
  // Define the step sizes for movement and rotation
  float moveSpeed = 0.1f;
  float rotationSpeed = 0.1f;

  float baseSpeed = 0;
  float multiSpeed = 20;
signals:
  void dataUpdated(CameraData data);
};

#endif // CAMERA_H
