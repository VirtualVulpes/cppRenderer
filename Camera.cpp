#include "Camera.h"

#include <algorithm>

#include "glm/gtc/matrix_transform.hpp"

constexpr glm::vec3 kWorldUp(0.0f, 1.0f, 0.0f);

Camera::Camera(glm::vec3 pos, float pitch, float yaw)
  : pos_(pos)
  , pitch_(pitch)
  , yaw_(yaw) {
  UpdateCameraVectors();
}

glm::mat4 Camera::GetView() const {
  return glm::lookAt(pos_, pos_ + front_, up_);
}

glm::mat4 Camera::GetProjection(float aspect_ratio) const {
  return glm::perspective(glm::radians(fov_), aspect_ratio, z_near_, z_far_);
}

void Camera::Update(const InputState &input, float delta_time) {
  MoveCamera(input.keys, delta_time);
  RotateCamera(input.mouse);
}

void Camera::MoveCamera(const InputState::Keys &input, float delta_time) {
  glm::vec3 movement{};

  if (input.forward) movement += front_;
  if (input.backward) movement -= front_;
  if (input.right) movement += right_;
  if (input.left) movement -= right_;

  if (glm::length(movement) > 0.0f)
    pos_ += glm::normalize(movement) * delta_time * speed_;
}

void Camera::RotateCamera(const InputState::Mouse &input) {
  float x_offset = input.x_pos - input.x_last;
  float y_offset = input.y_pos - input.y_last;

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
