//
// Created by sjfoc on 2/8/2026.
//

#ifndef UNTITLED_CAMERA_H
#define UNTITLED_CAMERA_H
#include <glm/glm.hpp>

struct InputState;

class Camera
{
public:
    Camera(glm::vec3 pos, float pitch, float yaw);
    glm::mat4 getViewMatrix() const;
    void processInput(const InputState& state, float deltaTime);
private:
    glm::vec3 m_pos;
    float m_pitch;
    float m_yaw;

    glm::vec3 m_front;
    glm::vec3 m_right;
    glm::vec3 m_up;

    float m_speed { 3.0 };

    void updateCameraVectors();
};


#endif //UNTITLED_CAMERA_H