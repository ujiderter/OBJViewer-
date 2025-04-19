#pragma once
#include "OpenGLContext.h"
#include "ModelRenderer.h"

class OpenGLWindow {
private:
    HWND hwnd = nullptr;
    std::unique_ptr<OpenGLContext> context;
    std::unique_ptr<ModelRenderer> renderer;

    static std::unordered_map<HWND, OpenGLWindow*> windowInstances;

    bool mouseDown = false;
    int lastMouseX = 0;
    int lastMouseY = 0;

    int width = 800;
    int height = 600;

public:
    OpenGLWindow() :
        context(std::make_unique<OpenGLContext>()),
        renderer(std::make_unique<ModelRenderer>()) {
    }

    ~OpenGLWindow() {
        destroy();
    }

    bool create(const std::string& title) {
        WNDCLASSEX wc = {
            sizeof(WNDCLASSEX),
            CS_HREDRAW | CS_VREDRAW | CS_OWNDC,  // Стиль класса окна
            WindowProc,                         // Оконная процедура
            0,                                  // Дополнительные байты класса
            0,                                  // Дополнительные байты окна
            GetModuleHandle(nullptr),           // Дескриптор экземпляра
            LoadIcon(nullptr, IDI_APPLICATION), // Иконка
            LoadCursor(nullptr, IDC_ARROW),     // Курсор
            nullptr,                            // Фон
            nullptr,                            // Имя меню
            L"OGLWindowClass",                   // Имя класса
            LoadIcon(nullptr, IDI_APPLICATION)  // Маленькая иконка
        };

        if (!RegisterClassEx(&wc)) {
            return false;
        }

        std::wstring wideTitle(title.length(), L' ');
        std::copy(title.begin(), title.end(), wideTitle.begin());

        // Создание окна
        hwnd = CreateWindowEx(
            WS_EX_APPWINDOW | WS_EX_WINDOWEDGE,    // Расширенный стиль
            L"OGLWindowClass",                      // Класс окна
            wideTitle.c_str(),                         // Заголовок
            WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
            CW_USEDEFAULT, CW_USEDEFAULT,         // Позиция x, y
            width, height,                        // Ширина, высота
            nullptr,                              // Родительское окно
            nullptr,                              // Меню
            GetModuleHandle(nullptr),             // Экземпляр
            nullptr                               // Параметр lParam CreateWindow
        );

        if (!hwnd) {
            return false;
        }

        windowInstances[hwnd] = this;

        HDC hdc = GetDC(hwnd);

        try {
            context->create(hdc);
            renderer->initialize();

            ShowWindow(hwnd, SW_SHOW);
            UpdateWindow(hwnd);

            return true;
        }
        catch (const std::exception& e) {
            MessageBoxA(nullptr, e.what(), "OpenGL Error", MB_OK | MB_ICONERROR);
            ReleaseDC(hwnd, hdc);
            return false;
        }
    }

    int runMessageLoop() {
        MSG msg = {};

        while (true) {
            if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
                if (msg.message == WM_QUIT) {
                    break;
                }

                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
            else {
                render();
            }
        }

        return static_cast<int>(msg.wParam);
    }

    void setModel(std::shared_ptr<Model3D> model) {
        renderer->setModel(std::move(model));
    }

    void onResize(int newWidth, int newHeight) {
        width = newWidth;
        height = newHeight;

        if (renderer) {
            renderer->resize(width, height);
        }
    }

    void onMouseDown(int x, int y) {
        mouseDown = true;
        lastMouseX = x;
        lastMouseY = y;
    }

    void onMouseUp() {
        mouseDown = false;
    }

    void onMouseMove(int x, int y) {
        if (mouseDown) {
            int deltaX = x - lastMouseX;
            int deltaY = y - lastMouseY;

            renderer->updateRotation(static_cast<float>(deltaX), static_cast<float>(deltaY));

            lastMouseX = x;
            lastMouseY = y;
        }
    }

    void onMouseWheel(int delta) {
        renderer->updateZoom(static_cast<float>(delta) / 120.0f);
    }

    void render() {
        if (renderer) {
            renderer->render();
            context->swapBuffers();
        }
    }

    void destroy() {
        if (hwnd) {
            windowInstances.erase(hwnd);

            if (context) {
                context->destroy();
            }

            DestroyWindow(hwnd);
            hwnd = nullptr;
        }
    }

    static OpenGLWindow* getInstance(HWND hwnd) {
        auto it = windowInstances.find(hwnd);
        if (it != windowInstances.end()) {
            return it->second;
        }
        return nullptr;
    }
};
