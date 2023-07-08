#ifndef OBJECT3D_H
#define OBJECT3D_H
#include "E/Camera.h"
#include "E/EObject.h"
#include "E/Mesh.h"
#include "E/Transform.h"
#include "Shaders/Program.h"
namespace s21 {
class Object3D : public EObject {
 public:
  Object3D() = default;
  Object3D(const Mesh& mesh) : mesh_(mesh){};
  Object3D(const Mesh& mesh, Program& program) : mesh_(mesh) {
    SetProgram(program);
  };
  Object3D(const Mesh& mesh, Program& program, const Transform& transform)
      : mesh_(mesh), transform_(transform) {
    SetProgram(program);
  };
  virtual EObjectType GetType() const override { return type_; };
  void Draw(GLenum type, Camera* camera);

  void UploadMesh(const OBJ& obj, OBJImportStrategy* importer);

  void SetMesh(const Mesh& mesh);
  void SetProgram(Program& program);
  void SetTransform(const Transform& transform);
  Mesh& GetMesh() { return mesh_; };
  Transform& GetTrasform() { return transform_; };

 protected:
  EObjectType type_ = kObject3D;

 private:
  Mesh mesh_;
  Program* program_{};
  Transform transform_;
};

}  // namespace s21

#endif  // OBJECT3D_H
