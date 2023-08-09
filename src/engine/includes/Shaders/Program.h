#ifndef PROGRAM_H
#define PROGRAM_H

#ifndef GL_SILENCE_DEPRECATION
#define GL_SILENCE_DEPRECATION
#endif

#include <QOpenGLExtraFunctions>

#include "Shaders/FragmentShader.h"
#include "Shaders/VertexShader.h"
namespace s21 {
class Program : protected QOpenGLExtraFunctions {
 public:
  Program();
  Program(const Program& other) { *this = other; }

  Program(Program&& other) { *this = other; }

  Program(const std::string& vertexFile, const std::string& fragmentFile);
  void Activate();
  void Delete();
  int GetUniform(const std::string& name);
  static Program* Default();
  Program& operator=(const Program& other);
  Program& operator=(Program&& other);
  // GL Functions
  void Uniform1i(const char* name, int a);
  void Uniform1f(const char* name, float a);
  void Uniform2f(const char* name, float a, float b);
  void Uniform3f(const char* name, float a, float b, float c);
  void UniformMatrix4fv(const char* name, int count, bool normalize,
                        const float* data);
  void LineWidth(float width);

 private:
  GLuint ID_ = 0;
  void CopyProgram(const Program& other);
  void SetProgram(const std::string& vertexFile,
                  const std::string& fragmentFile);
  bool LinkSuccessful();
  void PrintProgramError();
};
}  // namespace s21

#endif  // PROGRAM_H
