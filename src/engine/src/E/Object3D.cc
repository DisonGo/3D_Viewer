#include "E/Object3D.h"

namespace s21 {
void Object3D::Draw(GLenum type, Camera* camera) {
  if (!program_ || !camera) return;
  program_->Activate();
  transform_.LoadModelMatrix(program_);
  camera->Matrix(*program_, "u_camMatrix");
  if (type == GL_POINTS) {
    if (point_display_method_ == PointDisplayType::kNone) return;
    auto is_circle = point_display_method_ == PointDisplayType::kCircle;
    glUniform1i(program_->GetUniform("u_circlePoint"), is_circle);
    glUniform1f(program_->GetUniform("u_pointSize"), vertices_size_);
    glUniform3f(program_->GetUniform("u_prototype_color"),vertices_color_.redF(), vertices_color_.greenF(), vertices_color_.blueF());
  }
  if (type == GL_LINES) {
    auto is_dashed = line_display_type_ == LineDisplayType::kDashed;
    glUniform1i(program_->GetUniform("u_dashed"), is_dashed);
    glUniform1f(program_->GetUniform("u_lineWidth"), edges_thickness_);
    glUniform3f(program_->GetUniform("u_prototype_color"),edges_color_.redF(), edges_color_.greenF(), edges_color_.blueF());
  }
  mesh_.Draw(type);
}

void Object3D::UploadMesh(const OBJ& obj, OBJImportStrategy* importer) {
  if (!importer) return;
  mesh_.Import(obj, importer);
}
void Object3D::SetTransform(const Transform& transform) {
  if (transform_ == transform) return;
  transform_ = transform;
  transform_.UpdateModel();
}
void Object3D::SetEdgesColor(QColor new_color) { edges_color_ = new_color; }

void Object3D::SetEdgesThickness(double new_thickness) {
  edges_thickness_ = new_thickness;
}

void Object3D::SetVerticesColor(QColor new_color) {
  vertices_color_ = new_color;
}

void Object3D::SetVerticesSize(double new_size) { vertices_size_ = new_size; }

void Object3D::SetDisplayMethod(PointDisplayType new_method) {
  point_display_method_ = new_method;
}

void Object3D::SetLineDisplayType(LineDisplayType new_type) {
  line_display_type_ = new_type;
}

void Object3D::SetProgram(Program& program) { program_ = &program; }

void Object3D::SetMesh(const Mesh& mesh) { mesh_ = std::move(mesh); }
}  // namespace s21
