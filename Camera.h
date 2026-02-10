//
// Created by sjfoc on 2/8/2026.
//

#ifndef UNTITLED_CAMERA_H
#define UNTITLED_CAMERA_H
#include <glm/glm.hpp>

#include "InputState.h"

struct InputState;

class Camera
{
public:
    Camera(glm::vec3 pos, float pitch, float yaw);
    glm::mat4 getViewMatrix() const;
    void processInput(const InputState& input, float deltaTime);
private:
    glm::vec3 m_pos;
    float m_pitch;
    float m_yaw;

    glm::vec3 m_front;
    glm::vec3 m_right;
    glm::vec3 m_up;

    float m_speed { 3.0 };
    float m_sensitivity { 3.0 };

    void updateCameraVectors();
    void moveCamera(const InputState::Keys& input, float deltaTime);
    void rotateCamera(const InputState::Mouse& input, float deltaTime);
};


#endif //UNTITLED_CAMERA_H