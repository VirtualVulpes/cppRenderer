#ifndef CPPRENDERER_SHADER_H
#define CPPRENDERER_SHADER_H
#include <string_view>
#include <string>
#include <unordered_map>

#include "glm/detail/type_mat.hpp"
#include "glm/detail/type_vec.hpp"

class Shader {
public:
  Shader(std::string_view vertPath, std::string_view fragPath);
  ~Shader();

  void SetMat4(std::string_view uniform, const glm::mat4& value) const;
  void SetVec3(std::string_view uniform, const glm::vec3& value) const;
  void SetVec2(std::string_view uniform, const glm::vec2& value) const;
  void SetFloat(std::string_view uniform, float value) const;
  void SetInt(std::string_view uniform, int value) const;
  void Use() const;

  enum class ShaderType { kVertex, kFragment, kProgram };

private:
  unsigned int id_;
  mutable std::unordered_map<std::string, int> uniform_cache_;

  void CheckCompilation(unsigned int shader, ShaderType type);
  int GetUniformLocation(std::string_view uniform) const;
  static std::string ReadFile(std::string_view path);
  static std::string ToString(ShaderType type);
};

#endif
