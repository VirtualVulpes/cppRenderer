#include "Mesh.h"

#include <vector>

#include "glad/glad.h"
#include "glm/detail/type_vec.hpp"
#include "glm/detail/type_vec2.hpp"
#include "glm/detail/type_vec3.hpp"

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices) {
  glGenVertexArrays(1, &vao_);
  glGenBuffers(1, &vbo_);
  glGenBuffers(1, &ebo_);

  glBindVertexArray(vao_);
  glBindBuffer(GL_ARRAY_BUFFER, vbo_);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), indices.data(), GL_STATIC_DRAW);
  tris_ = indices.size() / 3;

  // position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)  offsetof(Vertex, pos));
  glEnableVertexAttribArray(0);
  // normal attribute
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, normal));
  glEnableVertexAttribArray(1);
  // texture coord attribute
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)  offsetof(Vertex, uv));
  glEnableVertexAttribArray(2);

  // unbind
  glBindVertexArray(0);                            // vao
  glBindBuffer(GL_ARRAY_BUFFER, 0);         // vbo
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // ebo
}

Mesh::~Mesh() {
  glDeleteVertexArrays(1, &vao_);
  glDeleteBuffers(1, &vbo_);
  glDeleteBuffers(1, &ebo_);
}

void Mesh::Draw() const {
  glBindVertexArray(vao_);
  //glDrawArrays(GL_TRIANGLES, 0, tris_);
  glDrawElements(GL_TRIANGLES, tris_ * 3, GL_UNSIGNED_INT, nullptr);
}

void Mesh::CreateMesh(float vertices[]) {
  glBindVertexArray(vao_);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  tris_ = sizeof(vertices) / sizeof(vertices[0]) / 8;
}
