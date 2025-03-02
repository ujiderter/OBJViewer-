#ifndef OBJVIEWER_MTLPARSER_H
#define OBJVIEWER_MTLPARSER_H

#include <unordered_map>
#include <sstream>
#include <memory>
#include <optional>
#include <fstream>
#include "Model.h"


class MTLParser {
public:
    using MaterialMap = std::unordered_map<std::string, Material>;

    // Функция для загрузки MTL-файла
    static std::unique_ptr<MaterialMap> loadMTL(const std::string& filepath) {
        std::ifstream file(filepath);
        if (!file.is_open()) {
            return nullptr;
        }

        auto materials = std::make_unique<MaterialMap>();
        Material currentMaterial;

        std::string line;
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            std::string key;
            iss >> key;

            if (key == "newmtl") {
                if (!currentMaterial.name.empty()) {
                    (*materials)[currentMaterial.name] = currentMaterial;
                }
                iss >> currentMaterial.name;
            } else if (key == "Ka") {
                iss >> currentMaterial.ambientColor.r >> currentMaterial.ambientColor.g >> currentMaterial.ambientColor.b;
            } else if (key == "Kd") {
                iss >> currentMaterial.diffuseColor.r >> currentMaterial.diffuseColor.g >> currentMaterial.diffuseColor.b;
            } else if (key == "Ks") {
                iss >> currentMaterial.specularColor.r >> currentMaterial.specularColor.g >> currentMaterial.specularColor.b;
            } else if (key == "Ns") {
                iss >> currentMaterial.shininess;
            } else if (key == "d" || key == "Tr") {
                iss >> currentMaterial.dissolve;
            } else if (key == "illum") {
                int illumValue;
                iss >> illumValue;
                currentMaterial.illum = illumValue > 0;
            } else if (key == "map_Kd") {
                std::string texturePath;
                iss >> texturePath;
                currentMaterial.textureMap = texturePath;
            }
        }

        // Добавляем последний материал
        if (!currentMaterial.name.empty()) {
            (*materials)[currentMaterial.name] = currentMaterial;
        }

        file.close();
        return materials;
    }
};

#endif //OBJVIEWER_MTLPARSER_H
