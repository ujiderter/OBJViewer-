#ifndef OBJVIEWER__MODEL_H
#define OBJVIEWER__MODEL_H

#include <vector>
#include <memory>
#include "loaders/ILoader.h"

class Model {
private:
    std::vector<ILoader::Triangle> triangles; // Список треугольников
    std::unique_ptr<ILoader> loader;         // Загрузчик данных
public:
    // Конструктор принимает загрузчик
    explicit Model(std::unique_ptr<struct OBJLoader> loader)
            : loader(std::move(loader)) {}

    // Метод для загрузки модели из файла
    void loadModel(const std::string& filePath) {
        if (!loader) {
            throw std::runtime_error("Loader is not set.");
        }
        triangles = loader->loadModel(filePath);
    }

    // Метод для получения списка треугольников
    [[nodiscard]] const std::vector<ILoader::Triangle>& getTriangles() const {
        return triangles;
    }
};

#endif //OBJVIEWER__MODEL_H
