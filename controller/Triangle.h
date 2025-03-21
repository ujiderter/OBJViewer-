#ifndef OBJVIEWER__TRIANGLE_H
#define OBJVIEWER__TRIANGLE_H

#include <array>
#include "../model/math/Vector3D.h"

class Triangle {
public:
    struct Vertex {
        float x, y, z;
    };

private:
    std::array<VecMath::Vector3D<float>, 3> normals; // Нормаль треугольника
    std::array<Vertex, 3> vertices; // Вершины треугольника
    VecMath::Vector3D<float> averageNormal; // Усредненная нормаль треугольника
public:
    // Конструктор
    Triangle(Vertex v1, Vertex v2, Vertex v3,
             VecMath::Vector3D<float> vn1,
             VecMath::Vector3D<float> vn2,
             VecMath::Vector3D<float> vn3)
            : vertices{v1, v2, v3}, normals{vn1, vn2, vn3} {
        // Вычисляем усредненную нормаль
        averageNormal = (vn1 + vn2 + vn3) / 3.0f;
        averageNormal.normalize();
    }

    // Метод для получения вершин
    [[nodiscard]] const std::array<Vertex, 3>& getVertices() const {
        return vertices;
    }

    // Метод для получения нормалей
    [[nodiscard]] const std::array<VecMath::Vector3D<float>, 3>& getNormals() const {
        return normals;
    }

    // Метод для получения усредненной нормали
    [[nodiscard]] const VecMath::Vector3D<float>& getAverageNormal() const {
        return averageNormal;
    }

    // Перевод вершин в центр экрана
    void translate(int centerX, int centerY) {
        vertices[0].x += centerX;
        vertices[0].y += centerY;
        vertices[1].x += centerX;
        vertices[1].y += centerY;
        vertices[2].x += centerX;
        vertices[2].y += centerY;
    }

    // Масштабирование вершин
    void scale(float scaleFactor) {
        vertices[0].x *= scaleFactor;
        vertices[0].y *= scaleFactor;
        vertices[1].x *= scaleFactor;
        vertices[1].y *= scaleFactor;
        vertices[2].x *= scaleFactor;
        vertices[2].y *= scaleFactor;
    }

    // Проверка видимости (направление нормали)
    [[nodiscard]] bool isVisible() const {
        return averageNormal.z >= 0;
    }

    // Вычисление интенсивности света
    [[nodiscard]] float computeLightIntensity(const VecMath::Vector3D<float>& lightDirection) const {
        return std::max(0.0f, averageNormal.dot(lightDirection));
    }

    // Метод для вычисления максимальной z-координаты
    [[nodiscard]] float getMaxZ() const {
        return std::max(vertices[0].z, std::max(vertices[1].z, vertices[2].z));
    }

    // Перегрузка оператора "<" для сравнения треугольников
    bool operator<(const Triangle& other) const {
        return getMaxZ() > other.getMaxZ();
    }
};

#endif //OBJVIEWER__TRIANGLE_H
