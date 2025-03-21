#ifndef OBJVIEWER__ILOADER_H
#define OBJVIEWER__ILOADER_H

#include <vector>
#include <string>

class ILoader {
public:
    struct Triangle {
        float v1x, v1y, v1z; // Координаты первой вершины
        float v2x, v2y, v2z; // Координаты второй вершины
        float v3x, v3y, v3z; // Координаты третьей вершины

        float vn1x, vn1y, vn1z; // Нормаль для первой вершины
        float vn2x, vn2y, vn2z; // Нормаль для второй вершины
        float vn3x, vn3y, vn3z; // Нормаль для третьей вершины
    };

    virtual ~ILoader() = default;

    [[nodiscard]] virtual std::vector<Triangle> loadModel(const std::string& filePath) const = 0;
};

#endif //OBJVIEWER__ILOADER_H
