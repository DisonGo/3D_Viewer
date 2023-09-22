#ifndef GLOBJECT_H
#define GLOBJECT_H

#include <Logger.h>

#include <QOpenGLExtraFunctions>
#include <QOpenGLFunctions>
namespace s21 {
class GLObject : public QOpenGLExtraFunctions {
 public:
  explicit GLObject() { initializeOpenGLFunctions(); };

 protected:
  GLuint ID_ = 0;
  virtual void Bind() = 0;
  virtual void Unbind() = 0;
  virtual void Delete() = 0;
  virtual ~GLObject() = default;
};
}  // namespace s21

#endif  // GLOBJECT_H
