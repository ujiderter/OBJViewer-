#ifndef OBJVIEWER_MODEL_H
#define OBJVIEWER_MODEL_H

#include <string>

class Material {
public:
    std::string name; // Название материала

    // Цвета (RGB)
    struct Color {
        float r, g, b;
    };

    Color ambientColor = {0.2f, 0.2f, 0.2f}; // Амбиентный цвет
    Color diffuseColor = {0.8f, 0.8f, 0.8f}; // Диффузный цвет
    Color specularColor = {0.0f, 0.0f, 0.0f}; // Спекулярный цвет

    float shininess = 0.0f; // Яркость спекуляций
    float dissolve = 1.0f; // Прозрачность (0 - полностью прозрачный, 1 - непрозрачный)
    [[maybe_unused]] bool illum = false; // Использование освещения

    std::string textureMap; // Файл текстуры
};

#endif //OBJVIEWER_MODEL_H
