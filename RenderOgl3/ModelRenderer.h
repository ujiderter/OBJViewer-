#pragma once
#include <memory>
#include <vector>
#include "ShaderProgram.h"
#include "MeshBuffer.h"
#include "CubeBuffer.h"
#include "Camera.h"
#include "Shaders.h"
#include "../models/Model3D.h"

class ModelRenderer {
private:
    std::shared_ptr<Model3D> model;
    std::unique_ptr<ShaderProgram> shader;
    std::vector<std::unique_ptr<MeshBuffer>> meshBuffers;
    std::unique_ptr<CubeBuffer> cubeBuffer;
    std::unique_ptr<Camera> camera;

public:
    ModelRenderer();
    void setModel(std::shared_ptr<Model3D> newModel);
    void initialize();
    void resize(int width, int height);
    void updateRotation(float deltaX, float deltaY);
    void updateZoom(float deltaZ);
    void render();
};