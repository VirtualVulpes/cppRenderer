cmake_minimum_required(VERSION 3.5)
project(renderer)

set(CMAKE_CXX_STANDARD 20)

#GLAD Library
add_library(glad external/glad/src/glad.c)
target_include_directories(glad PUBLIC external/glad/include)

# GLFW
add_subdirectory(external/glfw)

# Executable
add_executable(untitled main.cpp
        SHADER.h
        stb_image_impl.cpp
        Camera.cpp
        Camera.h
        InputState.h
)

# Link GLAD and GLFW
target_link_libraries(untitled PRIVATE glad glfw)

# GLM
target_include_directories(untitled PRIVATE external)