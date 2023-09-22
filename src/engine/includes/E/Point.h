#ifndef POINT_H
#define POINT_H
#include "E/Object3D.h"
#include "Logger.h"
namespace s21 {
class Point : public Object3D {
  using Vector3D = godison::vectors::Vector3D;

 public:
  explicit Point(float x = 0, float y = 0, float z = 0);
  Point(const Vector3D& position);
  virtual void Draw(GLenum type, Camera* camera) override;

 private:
  Logger logger_{"Point"};
  void CreateMesh(const Vector3D& position);
  Vector3D point_color_{255, 0, 0};
};
}  // namespace s21
#endif  // POINT_H
