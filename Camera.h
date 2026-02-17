#ifndef CPPRENDERER_CAMERA_H
#define CPPRENDERER_CAMERA_H

#include <glm/glm.hpp>

#include "InputState.h"

struct InputState;

class Camera {
public:
  Camera(glm::vec3 pos, float pitch, float yaw, float aspect_ratio);

  glm::mat4 GetViewMatrix() const;
  glm::mat4 GetProjectionMatrix() const;
  glm::vec3 GetPosition() const { return pos_; }

  void Update(const InputState &input, float delta_time);
  void MarkProjectionDirty();
  bool IsProjectionDirty() const { return is_projection_dirty_; }
  void ClearProjectionDirtyFlag() { is_projection_dirty_ = false; }
  void SetAspectRatio(float aspect_ratio);
  glm::vec2 GetZPlanes() const { return {z_near_, z_far_}; };

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
  float z_far_{1000.0f};

  float aspect_ratio_;
  bool is_projection_dirty_{true};

  void UpdateCameraVectors();
  void MoveCamera(const InputState::Keys &input, float delta_time);
  void RotateCamera(const InputState::Mouse &input);
};

#endif
