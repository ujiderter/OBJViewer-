#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
private:
    glm::mat4 projectionMatrix;
    glm::mat4 viewMatrix;
    float rotationX = 0.0f;
    float rotationY = 0.0f;
    float zoom = -5.0f;

public:
    void resize(int width, int height);
    void updateRotation(float deltaX, float deltaY);
    void updateZoom(float deltaZ);
    void updateViewMatrix();

    const glm::mat4& getProjectionMatrix() const;
    const glm::mat4& getViewMatrix() const;
};