#include "ShaderManager.h"
#include <filesystem>

ShaderManager *ShaderManager::instance = NULL;
ShaderManager *ShaderManager::GetInstance()
{
    if (instance == NULL)
    {
        instance = new ShaderManager();
    }
    return instance;
}

std::string readShader(const char *vertexShaderFile)
{
    std::filesystem::path path(vertexShaderFile);

    std::cout << "Trying to read: "
              << std::filesystem::absolute(path)
              << '\n';

    std::string content;
    std::ifstream fileStream(vertexShaderFile, std::ios::in);

    if (!fileStream.is_open())
    {
        std::cerr << "Could not read file " << vertexShaderFile << ". File does not exist." << std::endl;
        return "";
    }

    std::string line = "";
    while (!fileStream.eof())
    {
        std::getline(fileStream, line);
        content.append(line + "\n");
    }

    fileStream.close();
    return content;
}

void ShaderManager::compileAndLink(const char *vertexFile, const char *fragmentFile)
{
    std::string vsTemp = readShader(vertexFile);
    std::string fsTemp = readShader(fragmentFile);

    const char *vertexShaderSource = vsTemp.c_str();
    const char *fragmentShaderSource = fsTemp.c_str();

    // unsigned int vertexShader;
    // vertexShader = glCreateShader(GL_VERTEX_SHADER);
    // glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    // glCompileShader(vertexShader);

    // unsigned int fragmentShader;
    // fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    // glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    // glCompileShader(fragmentShader);

    // shaderProgram = glCreateProgram();
    // glAttachShader(shaderProgram, vertexShader);
    // glAttachShader(shaderProgram, fragmentShader);
    // glLinkProgram(shaderProgram);

    // glDeleteShader(vertexShader);
    // glDeleteShader(fragmentShader);

    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(
        vertexShader,
        1,
        &vertexShaderSource,
        NULL);
    glCompileShader(vertexShader);

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(
        fragmentShader,
        1,
        &fragmentShaderSource,
        NULL);
    glCompileShader(fragmentShader);

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

unsigned int ShaderManager::getShaderProgram()
{
    return shaderProgram;
}

void ShaderManager::destroy()
{
    if (instance)
    {
        delete instance;
    }
    instance = NULL;
}