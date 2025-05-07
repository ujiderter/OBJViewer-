#pragma once
#include <GL/glew.h>

class CubeBuffer {
private:
    GLuint vao, vbo, ebo;

public:
    CubeBuffer();
    ~CubeBuffer();

    void render() const;
};