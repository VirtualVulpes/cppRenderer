#ifndef CPPRENDERER_COLOR_H
#define CPPRENDERER_COLOR_H
#include "glm/detail/type_vec.hpp"
#include "glm/detail/type_vec3.hpp"

namespace Color {
  constexpr glm::vec3 Red {1.0f, 0.0f, 0.0f};
  constexpr glm::vec3 Green {0.0f, 1.0f, 0.0f};
  constexpr glm::vec3 Blue {0.0f, 0.0f, 1.0f};
  constexpr glm::vec3 White {1.0f, 1.0f, 1.0f};
  constexpr glm::vec3 Black {0.0f, 0.0f, 0.0f};
  constexpr glm::vec3 SkyBlue {0.471f, 0.655f, 1.0f};

}
#endif