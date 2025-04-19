#include <unordered_map>
#include "renders/OpenGLWindow.h"
#include "models/ModelManager.h"


// Инициализация статического члена
std::unordered_map<HWND, OpenGLWindow*> OpenGLWindow::windowInstances;

// Оконная процедура
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    OpenGLWindow* window = OpenGLWindow::getInstance(hwnd);

    switch (uMsg) {
    case WM_SIZE:
        if (window) {
            RECT clientRect;
            GetClientRect(hwnd, &clientRect);
            window->onResize(clientRect.right - clientRect.left, clientRect.bottom - clientRect.top);
        }
        return 0;

    case WM_LBUTTONDOWN:
        if (window) {
            window->onMouseDown(LOWORD(lParam), HIWORD(lParam));
        }
        return 0;

    case WM_LBUTTONUP:
        if (window) {
            window->onMouseUp();
        }
        return 0;

    case WM_MOUSEMOVE:
        if (window) {
            window->onMouseMove(LOWORD(lParam), HIWORD(lParam));
        }
        return 0;

    case WM_MOUSEWHEEL:
        if (window) {
            window->onMouseWheel(GET_WHEEL_DELTA_WPARAM(wParam));
        }
        return 0;

    case WM_CLOSE:
        PostQuitMessage(0);
        return 0;

    case WM_DESTROY:
        return 0;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

// Пример использования
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    try {
        // Создаем менеджер моделей
        ModelManager modelManager;

        // Пытаемся загрузить модель
        std::shared_ptr<Model3D> model;
        try {
            model = modelManager.loadModel("example.obj");
        }
        catch (const std::exception& e) {
            MessageBoxA(nullptr, e.what(), "Model Loading Error", MB_OK | MB_ICONWARNING);
            // Продолжаем без модели, будет отрисован куб
        }

        // Создаем окно
        auto window = std::make_unique<OpenGLWindow>();
        if (!window->create("3D Model Viewer")) {
            MessageBoxA(nullptr, "Failed to create OpenGL window", "Error", MB_OK | MB_ICONERROR);
            return 1;
        }

        // Устанавливаем модель
        if (model) {
            window->setModel(model);
        }

        // Запускаем цикл сообщений
        return window->runMessageLoop();
    }
    catch (const std::exception& e) {
        MessageBoxA(nullptr, e.what(), "Error", MB_OK | MB_ICONERROR);
        return 1;
    }
}