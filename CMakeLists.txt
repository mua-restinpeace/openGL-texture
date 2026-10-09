cmake_minimum_required(VERSION 3.20)

project(HelloOpenGL LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# GLFW installed through MSYS2
find_package(glfw3 REQUIRED)

# GLAD
add_library(glad STATIC
    external/glad/src/gl.c
)

target_include_directories(glad PUBLIC
    external/glad/include
)

# Our program
add_executable(HelloOpenGL
    src/main.cpp
    src/shaderManager.cpp
)

target_link_libraries(HelloOpenGL
    PRIVATE
    glad
    glfw
    opengl32
)