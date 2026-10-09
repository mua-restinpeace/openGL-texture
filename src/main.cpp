#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include <iostream>
#include <filesystem>
#include "ShaderManager.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

int main(){
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW\n";
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(
        800,
        600,
        "OpenGL",
        nullptr,
        nullptr);

    if (!window)
    {
        std::cerr << "Failed to create GLFW window\n";
        return -1;
    }

    glfwMakeContextCurrent(window);

    int version = gladLoadGL(glfwGetProcAddress);
    if (version == 0) {
        std::cerr << "Failed to initialize GLAD\n";
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }

    std::cout << "Working directory: "
          << std::filesystem::current_path()
          << '\n';

    ShaderManager::GetInstance()->compileAndLink("resources/sample.vs", "resources/sample.fs");

    float vertices[] = {
    //posisi           //warna              //uv koordinat teksture
    0.0f,-0.5f,0.0f,   1.0f, 0.0f, 0.0f,    0.0f,0.0f,
    0.0f,0.5f,0.0f,    0.0f, 1.0f, 0.0f,    0.0f,1.0f,
    0.5f,-0.5f,0.0f,   0.0f, 0.0f, 1.0f,    1.0f,0.0f,
    0.5f,0.5f,0.0f,    1.0f, 0.0f, 0.0f,    1.0f,1.0f,
    };

    unsigned int indices[] = {
        0, 1, 2,
        1, 2, 3,  
    };
    
    // declaration
    unsigned int VBO;
    unsigned int VAO;
    unsigned int EBO;
    unsigned int texture;

    //generate buffer
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    // texture warping
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // texture filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // load texture image
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    std::string fileTex = "resources/basecolor.png";
    unsigned char* data = stbi_load(fileTex.c_str(), &width, &height, &nrChannels, 0);
    if (data){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cerr << "Failed to load texture\n";
    }

    stbi_image_free(data);


    // bind VBO to VAO
    glBindVertexArray(VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        sizeof(indices),
        indices,
        GL_STATIC_DRAW        
    );

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(vertices),
        vertices,
        GL_STATIC_DRAW
    );
    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        8 * sizeof(float),
        (void *)0);
    glEnableVertexAttribArray(0);


    glVertexAttribPointer(
        1,
        3,
        GL_FLOAT,
        GL_FALSE,
        8 * sizeof(float),
        (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(
        2,
        2,
        GL_FLOAT,
        GL_FALSE,
        8 * sizeof(float),
        (void *)(6 * sizeof(float))
    );
    glEnableVertexAttribArray(2);

    // bind texture
    glBindTexture(GL_TEXTURE_2D, texture);

    unsigned int shaderProgram = ShaderManager::GetInstance()->getShaderProgram();

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(
            0.1f,
            0.1f,
            0.1f,
            1.0f);

        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(shaderProgram);

        float timeValue = glfwGetTime();
        float greenValue = sin(timeValue) / 2.0f + 0.5f;
        int vertexColorLocation = glGetUniformLocation(shaderProgram, "mycolor");
        glUniform1f(vertexColorLocation, greenValue);

        glBindVertexArray(VAO);

        glDrawElements(
            GL_TRIANGLES,
            sizeof(indices),
            GL_UNSIGNED_INT,
            0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &VAO);
    glDeleteBuffers(1, &EBO);
    ShaderManager::GetInstance()->destroy();
    glfwDestroyWindow(window);
    glfwTerminate();

}