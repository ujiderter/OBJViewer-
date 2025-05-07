#pragma once
#include <GL/glew.h>
#include <string>

class ShaderProgram {
private:
    GLuint programID;

public:
    ShaderProgram(const char* vertexSource, const char* fragmentSource);
    ~ShaderProgram();

    void use() const;
    void setMat4(const std::string& name, const glm::mat4& mat) const;
    void setVec3(const std::string& name, const glm::vec3& vec) const;
};