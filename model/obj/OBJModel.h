#ifndef OBJVIEWER__OBJMODEL_H
#define OBJVIEWER__OBJMODEL_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <array>
#include <memory>
#include <string>
#include <tuple>
#include <algorithm>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <tuple>
#include <optional>
#include <variant>
#include <string_view>

class OBJModel {
private:
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
        std::vector<std::tuple<std::optional<int>, std::optional<int>, std::optional<int>>> vertexIndices;
    };
public:
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
                if (!(iss >> v.x >> v.y >> v.z)) {
                    std::cerr << "Error parsing vertex data." << std::endl;
                    continue;
                }
                model->vertices.push_back(v);
            } else if (type == "vn") {
                Normal n;
                if (!(iss >> n.x >> n.y >> n.z)) {
                    std::cerr << "Error parsing normal data." << std::endl;
                    continue;
                }
                model->normals.push_back(n);
            } else if (type == "vt") {
                TexCoord t;
                if (!(iss >> t.u >> t.v)) {
                    std::cerr << "Error parsing texture coordinate data." << std::endl;
                    continue;
                }
                model->texCoords.push_back(t);
            } else if (type == "f") {
                Face f;
                std::string token;
                while (iss >> token) {
                    std::optional<int> v, vt, vn;
                    std::istringstream tss(token);

                    // Разделение индексов по '/'
                    std::string subToken;
                    std::getline(tss, subToken, '/');
                    if (!subToken.empty()) {
                        v = std::stoi(subToken) - 1;
                    }

                    std::getline(tss, subToken, '/');
                    if (!subToken.empty()) {
                        vt = std::stoi(subToken) - 1;
                    }

                    std::getline(tss, subToken, '/');
                    if (!subToken.empty()) {
                        vn = std::stoi(subToken) - 1;
                    }

                    f.vertexIndices.emplace_back(v, vt, vn);
                }
                model->faces.push_back(f);
            } else if (type == "mtllib") {
                std::string mtlFile;
                if (!(iss >> mtlFile)) {
                    std::cerr << "Error parsing material library name." << std::endl;
                    continue;
                }
                model->mtlLib = mtlFile;
            }
        }

        file.close();
        return model;
    }

    [[nodiscard]] const std::vector<Face> &getFaces() const {
        return faces;
    }

    [[nodiscard]] const std::vector<Vertex> &getVertices() const {
        return vertices;
    }

    [[nodiscard]] const std::vector<Normal> &getNormals() const {
        return normals;
    }


private:
    std::vector<Vertex> vertices;
    std::vector<Normal> normals;
    std::vector<TexCoord> texCoords;
    std::vector<Face> faces;


    std::string mtlLib;
};

#endif //OBJVIEWER__OBJMODEL_H
