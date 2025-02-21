#include <iostream>
#include "OBJModel.h"

int main() {
    std::string filepath = "m.obj";
    auto model = OBJModel::loadOBJ(filepath);

    if (model) {
        model->printModelInfo();
    }

    return 0;
}