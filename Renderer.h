#ifndef OBJVIEWER_RENDERER_H
#define OBJVIEWER_RENDERER_H

#include <string>
#include <memory>
#include "OBJModel.h"

class Renderer {
public:
    virtual ~Renderer() = default;

    virtual bool initialize() = 0;

    virtual void render(const OBJModel& model) = 0;

    virtual void cleanup() = 0;

    virtual void handleEvents() = 0;

    virtual void run() = 0;
};

#endif //OBJVIEWER_RENDERER_H
