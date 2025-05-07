#include "Camera.h"

void Camera::resize(int width, int height) {
    projectionMatrix = glm::perspective(glm::radians(45.0f),
                                      static_cast<float>(width) / height,
                                      0.1f, 100.0f);
}

void Camera::updateRotation(float deltaX, float deltaY) {
    rotationY += deltaX * 0.5f;
    rotationX += deltaY * 0.5f;
}

void Camera::updateZoom(float deltaZ) {
    zoom += deltaZ * 0.1f;
}

void Camera::updateViewMatrix() {
    viewMatrix = glm::mat4(1.0f);
    viewMatrix = glm::translate(viewMatrix, glm::vec3(0.0f, 0.0f, zoom));
    viewMatrix = glm::rotate(viewMatrix, glm::radians(rotationX), glm::vec3(1.0f, 0.0f, 0.0f));
    viewMatrix = glm::rotate(viewMatrix, glm::radians(rotationY), glm::vec3(0.0f, 1.0f, 0.0f));
}

const glm::mat4& Camera::getProjectionMatrix() const {
    return projectionMatrix;
}

const glm::mat4& Camera::getViewMatrix() const {
    return viewMatrix;
}