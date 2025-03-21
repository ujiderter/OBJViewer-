#ifndef OBJVIEWER__CONTROLLER_H
#define OBJVIEWER__CONTROLLER_H

#include "IEventHandler.h"
#include "../render/Renderer.h"
#include "../model/loaders/OBJLoader.h"
#include "../model/Model.h"
#include "Transformer.h"

class Controller : public IEventHandler {
private:
    std::unique_ptr<Renderer> renderer;
    std::unique_ptr<Model> model;

public:
    Controller() {
        renderer = std::make_unique<WinAPIRenderer>();
        model = std::make_unique<Model>(std::make_unique<OBJLoader>());
        renderer->setEventHandler(this);
    }

    bool initializeRenderer() {
        return true;
    }

    void run() {
        renderer->run(); // Запускаем основной цикл рендерера
    }

    void handleEvent(const Event& event) override {
        switch (event.type) {
            case EventType::FileOpen: {
                const auto& filePath = std::get<std::string>(event.payload);
                try {
                    model->loadModel(filePath);
                    auto workingTriangles = Transformer::transformTriangles(model->getTriangles());
                    renderer->updateModel(workingTriangles);
                } catch (const std::exception& e) {
                    //MessageBox(nullptr, e.what(), L"Error", MB_ICONERROR | MB_OK);
                }
                break;
            }
            case EventType::WindowClose: {
                break;
            }
            default:
                break;
        }
    }
};
#endif //OBJVIEWER__CONTROLLER_H
