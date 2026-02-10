//
// Created by sjfoc on 2/8/2026.
//

#include "Camera.h"

#include <iostream>

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

void Camera::processInput(const InputState& input, float deltaTime)
{
    moveCamera(input.keys, deltaTime);
    rotateCamera(input.mouse, deltaTime);
}

void Camera::moveCamera(const InputState::Keys& input, float deltaTime)
{
    glm::vec3 movement {};

    if (input.forward)  movement += m_front;
    if (input.backward) movement -= m_front;
    if (input.right)    movement += m_right;
    if (input.left)     movement -= m_right;

    if (glm::length(movement) > 0.0f)
        m_pos += glm::normalize(movement) * deltaTime * m_speed;
}

void Camera::rotateCamera(const InputState::Mouse& input, float deltaTime)
{
    float xOffset = input.xPos - input.xLast;
    float yOffset = input.yPos - input.yLast;

    std::cout << input.xPos << ": " << input.xLast << '\n';

    m_yaw += xOffset * deltaTime * m_sensitivity;
    m_pitch -= yOffset * deltaTime * m_sensitivity;
    updateCameraVectors();
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
