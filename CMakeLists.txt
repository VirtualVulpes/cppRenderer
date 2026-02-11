cmake_minimum_required(VERSION 3.5)
project(cppRenderer)

set(CMAKE_CXX_STANDARD 20)

#GLAD Library
add_library(glad external/glad/src/glad.c)
target_include_directories(glad PUBLIC external/glad/include)

# GLFW
add_subdirectory(external/glfw)

# Executable
add_executable(cppRenderer main.cpp
        shader.h
        Camera.cpp
        Camera.h
        InputState.h
        Window.cpp
        Window.h
        InputManager.cpp
        InputManager.h
        Renderer.cpp
        Renderer.h
)

# Link GLAD and GLFW
target_link_libraries(cppRenderer PRIVATE glad glfw)

# GLM
target_include_directories(cppRenderer PRIVATE external)