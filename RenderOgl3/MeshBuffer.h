#pragma once
#include <GL/glew.h>
#include <memory>
#include "../models/Model3D.h"

class MeshBuffer {
private:
    GLuint vao, vbo;
    size_t vertexCount;

public:
    MeshBuffer(const std::shared_ptr<Mesh>& mesh);
    ~MeshBuffer();

    void render() const;
};