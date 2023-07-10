#include "E/Camera.h"

namespace s21 {
// Setters

void Camera::SetZRange(const QVector2D &newZRange) { z_range_ = newZRange; }
void Camera::SetFOV(float newFOV) { FOV_ = newFOV; }
void Camera::SetVw(int newVw) { vw_ = newVw; }
void Camera::SetVh(int newVh) { vh_ = newVh; }
void Camera::SetMoveSpeed(float newMoveSpeed) { move_speed_ = newMoveSpeed; }
void Camera::SetRotationSpeed(float newRotationSpeed) {
  rotation_speed_ = newRotationSpeed;
}
void Camera::SetOrientation(const QVector3D &newOrientation) {
  orientation_ = newOrientation;
}
void Camera::SetViewMode(ViewMode newViewMode) { view_mode_ = newViewMode; }
void Camera::SetFocusPoint(const QVector3D &newFocusPoint) {
  focus_point_ = newFocusPoint;
}
void Camera::SetPosition(const QVector3D &newPosition) {
  position_ = newPosition;
}
void Camera::SetMode(CameraMode newMode) { mode_ = newMode;}
void Camera::SetBox(const ParallelBox &newBox) { box_ = newBox; }
void Camera::SetLineWidth(double newLineWidth) { lineWidth_ = newLineWidth; }
void Camera::SetZoomFactor(double newZoomFactor)
{
  zoom_factor = newZoomFactor;
}
// Getters

Camera::CameraMode Camera::GetMode() const { return mode_; };
Camera::ViewMode Camera::GetViewMode() const { return view_mode_; };
const QVector3D &Camera::GetFocusPoint() const { return focus_point_; };
const QVector3D &Camera::GetPosition() const { return position_; };
const QVector3D &Camera::GetOrientation() const { return orientation_; };
const QVector2D &Camera::GetZRange() const { return z_range_; };
float Camera::GetFOV() const { return FOV_; };
int Camera::GetVw() const { return vw_; };
int Camera::GetVh() const { return vh_; };
float Camera::GetMoveSpeed() const { return move_speed_; };
float Camera::GetRotationSpeed() const { return rotation_speed_; };
const Camera::ParallelBox &Camera::GetBox() const { return box_; }
double Camera::GetLineWidth() const { return lineWidth_; };
double Camera::GetZoomFactor() const
{
  return zoom_factor;
}
}  // namespace s21
