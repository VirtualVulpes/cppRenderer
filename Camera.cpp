//
// Created by sjfoc on 2/8/2026.
//

#include "Camera.h"

#include "InputState.h"
#include "glm/gtc/matrix_transform.hpp"

Camera::Camera(glm::vec3 pos, float pitch, float yaw)
    : m_pos(pos)
    , m_pitch(pitch)
    , m_yaw(yaw)
{
    updateCameraVectors();
}

glm::mat4 Camera::getViewMatrix() const
{
    return glm::lookAt(m_pos, m_pos + m_front, m_up);
}

void Camera::processInput(const InputState& state, float deltaTime)
{
    glm::vec3 movement {};

    if (state.forward)  movement += m_front;
    if (state.backward) movement -= m_front;
    if (state.right)    movement += m_right;
    if (state.left)     movement -= m_right;

    if (glm::length(movement) > 0.0f)
        m_pos += glm::normalize(movement) * deltaTime * m_speed;
}

void Camera::updateCameraVectors()
{
    m_front.x = cos(glm::radians(m_yaw) * cos(glm::radians(m_pitch)));
    m_front.y = sin(glm::radians(m_pitch));
    m_front.z = sin(glm::radians(m_yaw) * cos(glm::radians(m_pitch)));
    m_front = glm::normalize(m_front);

    m_right = glm::normalize(glm::cross(m_front, glm::vec3(0.0, 1.0, 0.0)));
    m_up = glm::normalize(glm::cross(m_right, m_front));

}
