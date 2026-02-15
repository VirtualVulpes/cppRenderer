#include "Shader.h"

#include <format>
#include <fstream>
#include <sstream>

#include "glad/glad.h"
#include "glm/detail/type_mat4x4.hpp"
#include "glm/gtc/type_ptr.hpp"

Shader::Shader(std::string_view vertPath, std::string_view fragPath)
  : id_(glCreateProgram())
{
  const std::string vertSourceStr = ReadFile(vertPath);
  const std::string fragSourceStr = ReadFile(fragPath);

  const char* vertSource = vertSourceStr.c_str();
  const char* fragSource = fragSourceStr.c_str();

  unsigned int vert = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vert, 1, &vertSource, nullptr);
  glCompileShader(vert);
  CheckCompilation(vert, ShaderType::kVertex);

  unsigned int frag = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(frag, 1, &fragSource, nullptr);
  glCompileShader(frag);
  CheckCompilation(frag, ShaderType::kFragment);

  glAttachShader(id_, vert);
  glAttachShader(id_, frag);
  glLinkProgram(id_);
  CheckCompilation(id_, ShaderType::kProgram);

  glDeleteShader(vert);
  glDeleteShader(frag);
}

Shader::~Shader() {
  glDeleteProgram(id_);
}

void Shader::Use() const {
  glUseProgram(id_);
}

// Shader::Use() must be called first to correctly assign value
void Shader::SetMat4(std::string_view uniform, const glm::mat4& value) const {
  int location = GetUniformLocation(std::string(uniform));
  glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::SetVec3(std::string_view uniform, const glm::vec3& value) const {
  int location = GetUniformLocation(std::string(uniform));
  glUniform3fv(location, 1, glm::value_ptr(value));
}

void Shader::SetFloat(std::string_view uniform, float value) const {
  int location = GetUniformLocation(std::string(uniform));
  glUniform1f(location, value);
}

std::string Shader::ReadFile(std::string_view path) {
  std::ifstream file(path.data());
  if (!file)
    throw std::runtime_error(std::format("Failed to open file at {}", path));

  std::stringstream buffer;
  buffer << file.rdbuf();

  return buffer.str();
}

void Shader::CheckCompilation(unsigned int shader, ShaderType type) {
  int success;
  char infoLog[512];
  if (type == ShaderType::kVertex || type == ShaderType::kFragment) {
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
      glGetShaderInfoLog(shader, 512, nullptr, infoLog);
      throw std::runtime_error(std::format("Failed to compile shader of type {}\n{}\n", ToString(type), infoLog));
    }
  } else if (type == ShaderType::kProgram) {
    glGetProgramiv(id_, GL_LINK_STATUS, &success);
    if (!success) {
      glGetProgramInfoLog(id_, 512, nullptr, infoLog);
      throw std::runtime_error(std::format("Failed to link shader of type {}\n{}\n", ToString(type), infoLog));
    }
  } else {
    throw std::runtime_error("Invalid shader type");
  }
}

std::string Shader::ToString(ShaderType type) {
  switch (type) {
    case ShaderType::kVertex: return "vertex";
    case ShaderType::kFragment: return "fragment";
    case ShaderType::kProgram: return "program";
    default: return "unknown";
  }
}

int Shader::GetUniformLocation(std::string_view uniform) const {
  auto it = uniform_cache_.find(std::string(uniform));
  if (it != uniform_cache_.end())
    return it->second;

  int location = glGetUniformLocation(id_, uniform.data());
  uniform_cache_.emplace(std::string(uniform), location);
  return location;
}