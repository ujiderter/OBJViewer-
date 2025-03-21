#ifndef OBJVIEWER__TRANSFORMER_H
#define OBJVIEWER__TRANSFORMER_H

#include <vector>
#include "Triangle.h"
#include "../model/loaders/ILoader.h"

class Transformer {
public:
    // Преобразование базовых треугольников в рабочие
    static std::vector<Triangle> transformTriangles(
            const std::vector<ILoader::Triangle>& baseTriangles
    ) {
        std::vector<Triangle> transformedTriangles;

        for (const auto& baseTriangle : baseTriangles) {
            // Создаем вершины из базового треугольника
            Triangle::Vertex v1{baseTriangle.v1x, baseTriangle.v1y, baseTriangle.v1z};
            Triangle::Vertex v2{baseTriangle.v2x, baseTriangle.v2y, baseTriangle.v2z};
            Triangle::Vertex v3{baseTriangle.v3x, baseTriangle.v3y, baseTriangle.v3z};

            // Создаем нормали для каждой вершины
            VecMath::Vector3D<float> vn1{baseTriangle.vn1x, baseTriangle.vn1y, baseTriangle.vn1z};
            VecMath::Vector3D<float> vn2{baseTriangle.vn2x, baseTriangle.vn2y, baseTriangle.vn2z};
            VecMath::Vector3D<float> vn3{baseTriangle.vn3x, baseTriangle.vn3y, baseTriangle.vn3z};

            // Создаем рабочий треугольник
            Triangle triangle(v1, v2, v3, vn1, vn2, vn3);

            //triangle.normalize();
            // Добавляем рабочий треугольник в результат
            transformedTriangles.push_back(triangle);
        }

        return transformedTriangles;
    }
};

#endif //OBJVIEWER__TRANSFORMER_H
