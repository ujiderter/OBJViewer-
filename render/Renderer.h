#ifndef OBJVIEWER_RENDERER_H
#define OBJVIEWER_RENDERER_H

#include <string>
#include <memory>
#include "../model/obj/OBJModel.h"
#include "../controller/IEventHandler.h"
#include "../model/loaders/ILoader.h"
#include "../controller/Triangle.h"

class Renderer {
public:
    virtual ~Renderer() = default;
    IEventHandler* eventHandler{nullptr};

    virtual void setEventHandler(IEventHandler* handler) = 0;

    [[nodiscard]] virtual bool initialize() = 0;
    virtual void render(const std::vector<Triangle>& triangles) = 0;
    virtual void cleanup() = 0;
    virtual void updateModel(std::vector<Triangle>) = 0;
    virtual void run() = 0;

    [[nodiscard]] virtual bool isInitialized() const { return true; }
};

#endif //OBJVIEWER_RENDERER_H
