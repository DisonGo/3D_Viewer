#include "Strategies/Import/OBJImportStandartStrategy.h"

namespace s21 {
VAO OBJImportStandartStrategy::Import(const OBJ& obj) const {
  VAO vao;
  vao.Bind();
  VBO vbo(GetVertexDataArray(obj));
  EBO ebo(obj.faces);
  vao.LinkAttrib(vbo, 0, 3, GL_FLOAT, sizeof(VertexData), NULL);
  vao.SetIndicesN(ebo.GetSize());
  vao.SetVerticesN(vbo.GetSize());
  vao.Unbind();
  vbo.Unbind();
  ebo.Unbind();
  return vao;
}

std::vector<VertexData> OBJImportStandartStrategy::GetVertexDataArray(
    const OBJ& obj) const {
  std::vector<VertexData> new_arr;
  auto vert_size = obj.vertices.size();
  auto norm_size = obj.normals.size();
  bool uneven = vert_size != norm_size;
  if (uneven) {
    for (auto& vertex : obj.vertices) new_arr.push_back({vertex, {0, 0, 0}});
  } else {
    for (size_t i = 0; i < vert_size; ++i)
      new_arr.push_back({obj.vertices[i], obj.normals[i]});
  }
  return new_arr;
}
}  // namespace s21
