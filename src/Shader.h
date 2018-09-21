#pragma once

#include <iostream>
#include <string>
#include <unordered_map>

#include <glm/glm.hpp>


struct ShaderProgramSource {
    std::string vertex;
    std::string fragment;
};


class Shader {
private:
    std::string m_Filepath;
    unsigned int m_RendererID;
    std::unordered_map<std::string, int> m_LocationCache;

    int getUniformLocation(const std::string &name);
    unsigned int compileShader(unsigned int type, const std::string& source);
    ShaderProgramSource parseShader(const std::string& filepath);
    unsigned int createShader(const std::string& vertexShader, const std::string& fragmentShader);

public:
    Shader(const std::string& filename);
    ~Shader();

    void bind() const;
    void unbind() const;

    void setUniform1i(const std::string& uniform, int v);
    void setUniform4f(const std::string& uniform, float v1, float v2, float v3, float v4);
    void setUniformMat4f(const std::string& uniform, glm::mat4& mat);
};
