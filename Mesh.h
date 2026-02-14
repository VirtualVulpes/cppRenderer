#ifndef CPPRENDERER_MESH_H
#define CPPRENDERER_MESH_H
#include <vector>

#include "Vertex.h"
#include "Geometry.h"

class Mesh {
public:
  Mesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices);

  template <typename Geo>
  Mesh(const Geo&) : Mesh(Geo::vertices, Geo::indices) {}

  ~Mesh();

  void Draw() const;
  void CreateMesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices);

private:
  unsigned int vao_;
  unsigned int vbo_;
  unsigned int ebo_;

  int tris_;
};

#endif
