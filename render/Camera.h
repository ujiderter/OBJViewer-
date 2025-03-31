#ifndef OBJVIEWER__CAMERA_H
#define OBJVIEWER__CAMERA_H

#include <vector>
#include "../model/math/Vector3D.h"
#include "../model/math/Matrix4x4.h"

class Camera {
public:
    virtual ~Camera() = default;

    // Установка позиции камеры
    virtual void setPosition(const VecMath::Vector3D<float>& position) = 0;
    [[nodiscard]] virtual VecMath::Vector3D<float> getPosition() const = 0;

    // Управление направлением камеры
    virtual void setTarget(const VecMath::Vector3D<float>& target) = 0;
    virtual void setDirection(const VecMath::Vector3D<float>& direction) = 0;
    [[nodiscard]] virtual VecMath::Vector3D<float> getDirection() const = 0;

    // Управление проекцией
    virtual void setPerspective(float fov, float aspectRatio, float nearPlane, float farPlane) = 0;
    virtual void setOrthographic(float left, float right, float bottom, float top, float nearPlane, float farPlane) = 0;

    // Получение матриц (предполагается, что у вас есть класс Matrix4x4)
    [[nodiscard]] virtual VecMath::Matrix4x4<float> getViewMatrix() const = 0;
    [[nodiscard]] virtual VecMath::Matrix4x4<float> getProjectionMatrix() const = 0;
    [[nodiscard]] virtual VecMath::Matrix4x4<float> getViewProjectionMatrix() const = 0;

    // Управление движением камеры
    virtual void moveForward(float distance) = 0;
    virtual void moveRight(float distance) = 0;
    virtual void moveUp(float distance) = 0;

    // Управление вращением
    virtual void rotate(float yaw, float pitch, bool constrainPitch = true) = 0;

    // Обновление состояния камеры
    virtual void update(float deltaTime) = 0;

    // Настройки камеры
    virtual void setMovementSpeed(float speed) = 0;
    virtual void setMouseSensitivity(float sensitivity) = 0;

    // Дополнительные полезные методы
    [[nodiscard]] virtual VecMath::Vector3D<float> getRight() const = 0;
    [[nodiscard]] virtual VecMath::Vector3D<float> getUp() const = 0;
    virtual void lookAt(const VecMath::Vector3D<float>& target) = 0;
};

#endif //OBJVIEWER__CAMERA_H
