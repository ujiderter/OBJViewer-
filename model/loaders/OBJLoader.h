#ifndef OBJVIEWER__OBJLOADER_H
#define OBJVIEWER__OBJLOADER_H

#include "ILoader.h"
#include "../obj/OBJModel.h"

class OBJLoader : public ILoader {
public:
    [[nodiscard]] std::vector<Triangle> loadModel(const std::string& filePath) const override {
        auto objModel = OBJModel::loadOBJ(filePath);
        if (!objModel) {
            throw std::runtime_error("Failed to load OBJ model from file: " + filePath);
        }
        return convertToTriangles(std::move(objModel));
    }

private:
    [[nodiscard]] static std::vector<Triangle> convertToTriangles(std::unique_ptr<OBJModel> objModel) {
        std::vector<Triangle> triangles;

        const auto& vertices = objModel->getVertices();
        const auto& normals = objModel->getNormals();
        const auto& faces = objModel->getFaces();

        for (const auto& face : faces) {
            if (face.vertexIndices.size() != 3) {
                continue;
            }

            // Извлекаем индексы вершин и нормалей
            auto [v0, vt0, vn0] = face.vertexIndices[0];
            auto [v1, vt1, vn1] = face.vertexIndices[1];
            auto [v2, vt2, vn2] = face.vertexIndices[2];

            // Проверяем, что индексы вершин существуют
            if (!v0.has_value() || !v1.has_value() || !v2.has_value()) {
                throw std::runtime_error("Invalid vertex indices in face.");
            }

            // Создаем треугольник
            Triangle triangle{};
            triangle.v1x = vertices[v0.value()].x;
            triangle.v1y = vertices[v0.value()].y;
            triangle.v1z = vertices[v0.value()].z;

            triangle.v2x = vertices[v1.value()].x;
            triangle.v2y = vertices[v1.value()].y;
            triangle.v2z = vertices[v1.value()].z;

            triangle.v3x = vertices[v2.value()].x;
            triangle.v3y = vertices[v2.value()].y;
            triangle.v3z = vertices[v2.value()].z;

            // Если есть нормали, добавляем их
            if (vn0.has_value() && vn1.has_value() && vn2.has_value()) {
                triangle.vn1x = normals[vn0.value()].x;
                triangle.vn1y = normals[vn0.value()].y;
                triangle.vn1z = normals[vn0.value()].z;

                triangle.vn2x = normals[vn1.value()].x;
                triangle.vn2y = normals[vn1.value()].y;
                triangle.vn2z = normals[vn1.value()].z;

                triangle.vn3x = normals[vn2.value()].x;
                triangle.vn3y = normals[vn2.value()].y;
                triangle.vn3z = normals[vn2.value()].z;
            } else {
                // Если нормали отсутствуют, используем нулевые значения
                triangle.vn1x = triangle.vn1y = triangle.vn1z = 0.0f;
                triangle.vn2x = triangle.vn2y = triangle.vn2z = 0.0f;
                triangle.vn3x = triangle.vn3y = triangle.vn3z = 0.0f;
            }

            triangles.push_back(triangle);
        }

        return triangles;
    }
};

#endif //OBJVIEWER__OBJLOADER_H
