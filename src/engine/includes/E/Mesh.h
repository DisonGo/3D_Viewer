#ifndef MESH_H
#define MESH_H
#include <QOpenGLFunctions>

#include "E/Camera.h"
#include "E/EObject.h"
#include "E/Transform.h"
#include "GL/EBO.h"
#include "GL/VAO.h"
#include "Shaders/Program.h"
#include "Strategies/Import/OBJImportStandartStrategy.h"
#include "Strategies/Import/OBJImportStrategy.h"
#include "Strategies/Import/OBJImportTriangleStrategy.h"
#include "Strategies/Import/OBJImportVertexOnlyStrategy.h"
#include "Strategies/Import/OBJImportWireframeStrategy.h"
using std::map;
namespace s21 {
class Mesh : public EObject, protected QOpenGLFunctions {
 public:
  Mesh();
  ~Mesh();
  Mesh(const OBJ& obj, OBJImportStrategy* importer = nullptr);
  void Draw(GLenum type);
  void Import(const OBJ& obj, OBJImportStrategy* importer);
  void SetBufferToggle(OBJImportStrategyType type, bool value);
  bool GetBufferToggle(OBJImportStrategyType type);
  const std::string& GetName() const {return name_;};
  virtual EObjectType GetType() const override { return type_; };

 protected:
  EObjectType type_ = EObjectType::kMesh;

 private:
  std::string name_ = "Mesh";
  map<OBJImportStrategyType, bool> buffer_toggle_{{kStandartImport, false},
                                                  {kWireframeImport, true},
                                                  {kTriangleImport, false},
                                                  {kVertexOnlyImport, false}};
  map<OBJImportStrategyType, VAO*> VAO_map_;
};
}  // namespace s21

#endif  // MESH_H
