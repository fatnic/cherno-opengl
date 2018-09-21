#include "Shader.h"

#include <GL/glew.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>


Shader::Shader(const std::string &filename)
    : m_Filepath(filename), m_RendererID(0)
{

     ShaderProgramSource source = parseShader(filename);
     m_RendererID = createShader(source.vertex, source.fragment);
     bind();
}

Shader::~Shader()
{
    glDeleteProgram(m_RendererID);
}

void Shader::bind() const
{
    glUseProgram(m_RendererID);
}

void Shader::unbind() const
{
    glUseProgram(0);
}

void Shader::setUniform1i(const std::string &uniform, int v)
{

    glUniform1i(getUniformLocation(uniform), v);
}

void Shader::setUniform4f(const std::string& uniform, float v1, float v2, float v3, float v4)
{
    glUniform4f(getUniformLocation(uniform), v1, v2, v3, v4);
}

void Shader::setUniformMat4f(const std::string& uniform, glm::mat4& mat)
{
    glUniformMatrix4fv(getUniformLocation(uniform), 1, GL_FALSE, &mat[0][0]);
}

int Shader::getUniformLocation(const std::string &name)
{
    if (m_LocationCache.find(name) != m_LocationCache.end())
        return m_LocationCache[name];

    int location = glGetUniformLocation(m_RendererID, name.c_str());

    if (location == -1)
        std::cout << "[Shader Error]: Uniform does not exist - " << name << std::endl;

    m_LocationCache[name] = location;
    return location;
}

ShaderProgramSource Shader::parseShader(const std::string& filepath) {
    std::ifstream stream(filepath);

    std::string line;
    std::stringstream ss[2];
    enum class ShaderType { NONE = -1, VERTEX = 0, FRAGMENT = 1 };
    ShaderType type = ShaderType::NONE;

    while(getline(stream, line)) {
        if (line.find("#shader") != std::string::npos) {
            if (line.find("vertex") != std::string::npos)
                type = ShaderType::VERTEX;
            else if (line.find("fragment") != std::string::npos)
                type = ShaderType::FRAGMENT;
        } else {
            ss[static_cast<int>(type)] << line << "\n";
        }
    }

    return { ss[0].str(), ss[1].str() };
}


unsigned int Shader::compileShader(unsigned int type, const std::string& source) {
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if(result == GL_FALSE) {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = static_cast<char*>(alloca(static_cast<unsigned long>(length) * sizeof(char)));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to commpile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") <<  " shader" << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(id);
        return 0;
    }

    return id;
}


unsigned int Shader::createShader(const std::string& vertexShader, const std::string& fragmentShader) {
    unsigned int program = glCreateProgram();
    unsigned int vs = compileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}
