#include "E/Transform.h"
bool Transform::operator==(const Transform &a) const {
  return (translate_ == a.translate_ && rotation_ == a.rotation_ &&
          scale_ == a.scale_);
}

const QVector3D &Transform::GetScale() const { return scale_; }
const QVector3D &Transform::GetRotation() const { return rotation_; }
const QVector3D &Transform::GetTranslate() const { return translate_; }

void Transform::SetScale(const QVector3D &newScale) {
  scale_ = newScale;
  UpdateScale();
}

void Transform::SetRotation(const QVector3D &newRotation) {
  rotation_ = newRotation;
  UpdateRotation();
}

void Transform::SetTranslate(const QVector3D &newTranslate) {
  translate_ = newTranslate;
  UpdateTranslate();
}

void Transform::UpdateScale() {
  modelScale_.setToIdentity();
  modelScale_.scale(scale_);
  awaitingLoadInProgram_ = true;
}

void Transform::UpdateRotation() {
  modelRot_.setToIdentity();
  modelRot_.rotate(rotation_.x(), 1, 0, 0);
  modelRot_.rotate(rotation_.y(), 0, 1, 0);
  modelRot_.rotate(rotation_.z(), 0, 0, 1);
  awaitingLoadInProgram_ = true;
}

void Transform::UpdateTranslate() {
  modelTranslate_.setToIdentity();
  modelTranslate_.translate(translate_);
  awaitingLoadInProgram_ = true;
}

void Transform::UpdateModel() {
  UpdateScale();
  UpdateRotation();
  UpdateTranslate();
}
void Transform::LoadModelMatrix(Program *program) {
  if (!awaitingLoadInProgram_) return;
  if (!program) return;
  int modelLoc = program->GetUniform("model");
  glUniformMatrix4fv(modelLoc, 1, GL_FALSE,
                     (modelTranslate_ * modelRot_ * modelScale_).data());
  awaitingLoadInProgram_ = false;
}
