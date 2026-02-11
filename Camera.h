#ifndef UNTITLED_CAMERA_H
#define UNTITLED_CAMERA_H

#include <glm/glm.hpp>
#include "InputState.h"

struct InputState;

class Camera {
public:
  Camera(glm::vec3 pos, float pitch, float yaw);

  glm::mat4 GetView() const;
  glm::mat4 GetProjection(float aspect_ratio) const;

  void Update(const InputState &input, float delta_time);

private:
  glm::vec3 pos_;
  float pitch_;
  float yaw_;

  glm::vec3 front_;
  glm::vec3 right_;
  glm::vec3 up_;

  float fov_{60.0f};
  float speed_{5.0f};
  float sensitivity_{0.05f};

  float z_near_{0.1f};
  float z_far_{100.0f};

  void UpdateCameraVectors();

  void MoveCamera(const InputState::Keys &input, float delta_time);

  void RotateCamera(const InputState::Mouse &input);
};

#endif //UNTITLED_CAMERA_H
