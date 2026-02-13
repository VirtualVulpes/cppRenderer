#include "Application.h"

#include <format>
#include <iostream>

#include "Camera.h"
#include "InputManager.h"
#include "Mesh.h"
#include "shader.h"
#include "Texture.h"

Application::Application()
    : window_{1280, 720, "Render Window"}
{

}

void Application::Run()
{
    Camera camera{glm::vec3(0, 0, 5), 0.0, -90.0};

    Shader shader("shaders/shader.vs", "shaders/shader.fs");

    Mesh mesh{};

    Texture texture{};
    texture.LoadFromFile("container.jpg");

    shader.use();

    InputManager input_manager{};

    float delta_time{0.0f};
    float last_frame{0.0f};

    while (!should_close_) {
        window_.PollEvents();

        float current_frame = glfwGetTime();
        delta_time = current_frame - last_frame;
        last_frame = current_frame;

        InputState input = input_manager.GetInput(window_);

        camera.Update(input, delta_time);
        if (input.keys.escape)
            Close();

        renderer_.Clear();

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture.GetId());

        shader.use();

        shader.setMat4("projection", camera.GetProjection(window_.GetAspectRatio()));
        shader.setMat4("view", camera.GetView());

        mesh.BindVertexArray();
        glm::mat4 model = glm::mat4(1.0f);
        shader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        window_.SwapBuffers();
    }
}

void Application::Close()
{
    should_close_ = true;
}