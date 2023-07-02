#ifndef OBJECT3D_H
#define OBJECT3D_H
#include "E/Camera.h"
#include "E/EObject.h"
#include "E/Mesh.h"
#include "E/Transform.h"
#include "Shaders/Program.h"
#include "Types/EdgeOBJ.h"
#include "Types/TriangleOBJ.h"
class Object3D : public EObject {
 public:
  Object3D(){};
  Object3D(const Mesh& mesh) : mesh_(mesh){};
  Object3D(const Mesh& mesh, Program& program) : mesh_(mesh) {
    SetProgram(program);
  };
  Object3D(const Mesh& mesh, Program& program, const Transform& transform)
      : mesh_(mesh), transform_(transform) {
    SetProgram(program);
  };
  EObjectType type = kObject3D;
  virtual EObjectType GetType() const override { return type; };
  void Draw(GLenum type, Camera* camera);

  void UploadMesh(const s21::EdgeOBJ& obj);
  void UploadMesh(const s21::TriangleOBJ& obj);

  void SetMesh(const Mesh& mesh);
  void SetProgram(Program& program);
  void SetTransform(const Transform& transform);
  Mesh& GetMesh() { return mesh_; };
  Transform& GetTrasform() { return transform_; };

 private:
  Mesh mesh_;
  Program* program_{};
  Transform transform_;
};

#endif  // OBJECT3D_H
