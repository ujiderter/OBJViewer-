#ifndef OBJVIEWER_RENDERER_H
#define OBJVIEWER_RENDERER_H

#include <string>
#include <memory>
#include "OBJModel.h"

class Renderer {
public:
    virtual ~Renderer() = default;

    [[nodiscard]] virtual bool initialize() = 0;
    virtual void render(const OBJModel& model) = 0;
    virtual void cleanup() = 0;
    virtual void handleEvents() = 0;
    virtual void run() = 0;

    virtual void loadModel(const std::string& filepath) {}
    [[nodiscard]] virtual bool isInitialized() const { return true; }
};

#endif //OBJVIEWER_RENDERER_H
