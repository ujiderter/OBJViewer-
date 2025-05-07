#include "ModelRenderer.h"

ModelRenderer::ModelRenderer() {
    shader = std::make_unique<ShaderProgram>(Shaders::vertexShaderSource, Shaders::fragmentShaderSource);
    cubeBuffer = std::make_unique<CubeBuffer>();
    camera = std::make_unique<Camera>();
}

void ModelRenderer::setModel(std::shared_ptr<Model3D> newModel) {
    model = std::move(newModel);
    meshBuffers.clear();
    if (model) {
        for (const auto& mesh : model->getMeshes()) {
            meshBuffers.push_back(std::make_unique<MeshBuffer>(mesh));
        }
    }
}

void ModelRenderer::initialize() {
    glewInit();
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
}

void ModelRenderer::resize(int width, int height) {
    glViewport(0, 0, width, height);
    camera->resize(width, height);
}

void ModelRenderer::updateRotation(float deltaX, float deltaY) {
    camera->updateRotation(deltaX, deltaY);
}

void ModelRenderer::updateZoom(float deltaZ) {
    camera->updateZoom(deltaZ);
}

void ModelRenderer::render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    camera->updateViewMatrix();
    shader->use();

    shader->setMat4("projection", camera->getProjectionMatrix());
    shader->setMat4("view", camera->getViewMatrix());
    shader->setVec3("lightPos", glm::vec3(10.0f, 10.0f, 10.0f));
    shader->setVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));

    glm::mat4 modelMatrix(1.0f);
    shader->setMat4("model", modelMatrix);

    if (model && !meshBuffers.empty()) {
        for (const auto& buffer : meshBuffers) {
            buffer->render();
        }
    } else {
        cubeBuffer->render();
    }

    glUseProgram(0);
}