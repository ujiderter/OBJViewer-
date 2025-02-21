#ifndef OBJVIEWER__OBJMODEL_H
#define OBJVIEWER__OBJMODEL_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <memory>
#include <string>
#include <tuple>
#include <algorithm>

class OBJModel {
public:
    // Структура для хранения вершины
    struct Vertex {
        float x, y, z;
    };

    // Структура для хранения нормали
    struct Normal {
        float x, y, z;
    };

    // Структура для хранения текстурных координат
    struct TexCoord {
        float u, v;
    };

    // Структура для хранения грани
    struct Face {
        std::vector<std::tuple<int, int, int>> vertexIndices;
    };

    // Функция для чтения OBJ-файла
    static std::unique_ptr<OBJModel> loadOBJ(const std::string& filepath) {
        std::ifstream file(filepath);
        if (!file.is_open()) {
            std::cerr << "Failed to open file: " << filepath << std::endl;
            return nullptr;
        }

        auto model = std::make_unique<OBJModel>();
        std::string line;

        while (std::getline(file, line)) {
            std::istringstream iss(line);
            std::string type;
            iss >> type;

            if (type == "v") {
                Vertex v;
                iss >> v.x >> v.y >> v.z;
                model->vertices.push_back(v);
            } else if (type == "vn") {
                Normal n;
                iss >> n.x >> n.y >> n.z;
                model->normals.push_back(n);
            } else if (type == "vt") {
                TexCoord t;
                iss >> t.u >> t.v;
                model->texCoords.push_back(t);
            } else if (type == "f") {
                Face f;
                std::string token;
                while (iss >> token) {
                    std::replace(token.begin(), token.end(), '/', ' ');
                    std::istringstream tss(token);
                    int v, vt, vn;
                    tss >> v >> vt >> vn;
                    f.vertexIndices.emplace_back(v - 1, vt - 1, vn - 1);
                }
                model->faces.push_back(f);
            } else if (type == "mtllib") {
                iss >> model->mtlLib;
            }
        }

        file.close();
        return model;
    }

    // Функция для вывода информации о модели
    void printModelInfo() const {
        std::cout << "Vertices: " << vertices.size() << std::endl;
        std::cout << "Normals: " << normals.size() << std::endl;
        std::cout << "TexCoords: " << texCoords.size() << std::endl;
        std::cout << "Faces: " << faces.size() << std::endl;
        std::cout << "MTL Lib: " << mtlLib << std::endl;
    }

private:
    std::vector<Vertex> vertices;
    std::vector<Normal> normals;
    std::vector<TexCoord> texCoords;
    std::vector<Face> faces;
    std::string mtlLib;
};

#endif //OBJVIEWER__OBJMODEL_H
