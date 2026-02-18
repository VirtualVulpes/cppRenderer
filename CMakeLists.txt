cmake_minimum_required(VERSION 3.5)
project(cppRenderer)

set(CMAKE_CXX_STANDARD 20)

#GLAD Library
add_library(glad external/glad/src/glad.c)
target_include_directories(glad PUBLIC external/glad/include)

# GLFW
add_subdirectory(external/glfw)
include_directories(renderer)

# Executable
add_executable(cppRenderer
        main.cpp
        Application.cpp
        Application.h
        stb_image.h
        Camera.cpp
        Camera.h
        input/InputState.h
        Window.cpp
        Window.h
        input/InputManager.cpp
        input/InputManager.h
        renderer/Renderer.cpp
        renderer/Renderer.h
        renderer/Mesh.cpp
        renderer/Mesh.h
        GameObject.h
        renderer/Texture.cpp
        renderer/Texture.h
        Transform.h
        WindowContext.h
        renderer/Shader.cpp
        renderer/Shader.h
        Geometry.h
        Vertex.h
        renderer/Framebuffer.cpp
        renderer/Framebuffer.h
        Light.h
        Material.h
        Renderable.h
        Color.h
        RenderContext.h
        ResourceHandler.h
        Settings.h
        input/Action.h
        input/KeyBindings.h
        TextureLoader.cpp
        TextureLoader.h
)

# Link GLAD and GLFW
target_link_libraries(cppRenderer PRIVATE glad glfw)

# GLM
target_include_directories(cppRenderer PRIVATE external)