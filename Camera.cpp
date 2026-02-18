#include "Camera.h"

#include <algorithm>

#include "glm/gtc/matrix_transform.hpp"

constexpr glm::vec3 kWorldUp(0.0f, 1.0f, 0.0f);

Camera::Camera(glm::vec3 pos, float pitch, float yaw, float aspect_ratio)
  : pos_(pos)
  , pitch_(pitch)
  , yaw_(yaw)
  , aspect_ratio_(aspect_ratio) {
  UpdateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix() const {
  return glm::lookAt(pos_, pos_ + front_, up_);
}

glm::mat4 Camera::GetProjectionMatrix() const {
  return glm::perspective(glm::radians(fov_), aspect_ratio_, z_near_, z_far_);
}

void Camera::Update(const InputState &input, float delta_time) {
  MoveCamera(input.keys, delta_time);
  RotateCamera(input);
}

void Camera::MoveCamera(const InputState::Keys& input, float delta_time) {
  glm::vec3 movement{};

  if (input.forward)  movement += front_;
  if (input.backward) movement -= front_;
  if (input.right)    movement += right_;
  if (input.left)     movement -= right_;
  if (input.up)       movement += kWorldUp;
  if (input.down)     movement -= kWorldUp;

  if (glm::length(movement) > 0.0f)
    pos_ += glm::normalize(movement) * delta_time * speed_;
}

void Camera::RotateCamera(const InputState& input) {
  float x_offset = input.mouse.x_pos - input.lastMouse.x_pos;
  float y_offset = input.mouse.y_pos - input.lastMouse.y_pos;

  yaw_ += x_offset * sensitivity_;
  pitch_ -= y_offset * sensitivity_;
  pitch_ = std::clamp(pitch_, -89.0f, 89.0f);
  UpdateCameraVectors();
}

void Camera::UpdateCameraVectors() {
  front_.x = cos(glm::radians(yaw_)) * cos(glm::radians(pitch_));
  front_.y = sin(glm::radians(pitch_));
  front_.z = sin(glm::radians(yaw_)) * cos(glm::radians(pitch_));
  front_ = glm::normalize(front_);

  right_ = glm::normalize(glm::cross(front_, kWorldUp));
  up_ = glm::normalize(glm::cross(right_, front_));
}


void Camera::MarkProjectionDirty() {
  is_projection_dirty_ = true;
}

void Camera::SetAspectRatio(float aspect_ratio) {
  aspect_ratio_ = aspect_ratio;
}
