#ifndef POINT_H
#define POINT_H
#include "E/Object3D.h"
namespace s21 {
class Point : public Object3D {
 public:
  Point(float x = 0, float y = 0, float z = 0);
  Point(const QVector3D& position);
  virtual void Draw(GLenum type, Camera* camera) override;

 private:
  void CreateMesh(const QVector3D& position);
  QColor point_color_{255, 0, 0};
};
}  // namespace s21
#endif  // POINT_H
