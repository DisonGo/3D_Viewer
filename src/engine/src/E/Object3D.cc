#include "E/Object3D.h"

namespace s21 {
void Object3D::Draw(GLenum type, Camera* camera) {
  if (!program_ || !camera) return;
  program_->Activate();
  transform_.LoadModelMatrix(program_);
  camera->Matrix(*program_, "u_camMatrix");
  float red = 1, green = 1, blue = 1;
  auto is_circle = point_display_method_ == PointDisplayType::kCircle;
  auto is_dashed = line_display_type_ == LineDisplayType::kDashed;
  if (type == GL_POINTS) {
    if (point_display_method_ == PointDisplayType::kNone) return;
    red = vertices_color_.redF();
    green = vertices_color_.greenF();
    blue = vertices_color_.blueF();
    glUniform1i(program_->GetUniform("u_circlePoint"), is_circle);
    glUniform1f(program_->GetUniform("u_pointSize"), vertices_size_);
  }
  if (type == GL_LINES) {
    red = edges_color_.redF();
    green = edges_color_.greenF();
    blue = edges_color_.blueF();
    glUniform1i(program_->GetUniform("u_dashed"), is_dashed);
    glUniform1i(program_->GetUniform("u_dashSize"), 3);
    glUniform1i(program_->GetUniform("u_gapSize"), 3);
    glUniform1f(program_->GetUniform("u_lineWidth"), edges_thickness_);
  }
  if (type == GL_TRIANGLES) red = green = blue = 0.8;
  glUniform3f(program_->GetUniform("u_prototype_color"), red, green, blue);
  mesh_.Draw(type);
  glUniform1i(program_->GetUniform("u_dashed"), false);
  glUniform1i(program_->GetUniform("u_circlePoint"), false);
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
