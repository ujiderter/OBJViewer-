#pragma once

#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <string>
#include <memory>
#include <stdexcept>
#include <functional>
#include <unordered_map>
#include <iostream>
#include "../models/Model3D.h"


LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

class OpenGLContext {
private:
    HDC deviceContext = nullptr;
    HGLRC renderingContext = nullptr;

public:
    OpenGLContext() = default;
    ~OpenGLContext() {
        destroy();
    }

    // Создание контекста OpenGL
    void create(HDC hdc) {
        deviceContext = hdc;

        // Настройки пикселей для OpenGL
        PIXELFORMATDESCRIPTOR pfd = {
            sizeof(PIXELFORMATDESCRIPTOR),
            1,                              // Версия
            PFD_DRAW_TO_WINDOW |           // Поддержка рисования в окно
            PFD_SUPPORT_OPENGL |           // Поддержка OpenGL
            PFD_DOUBLEBUFFER,              // Двойная буферизация
            PFD_TYPE_RGBA,                 // RGBA формат
            32,                            // 32-битный цвет
            0, 0, 0, 0, 0, 0,              // Игнорируем биты цвета
            0,                             // Нет альфа-буфера
            0,                             // Игнорируем бит сдвига
            0,                             // Нет аккумуляционного буфера
            0, 0, 0, 0,                    // Игнорируем биты аккумуляции
            24,                            // 24-битный Z-буфер
            8,                             // 8-битный буфер трафарета
            0,                             // Нет вспомогательного буфера
            PFD_MAIN_PLANE,                // Основной слой
            0,                             // Зарезервировано
            0, 0, 0                        // Игнорируем маски слоя
        };

        int pixelFormat = ChoosePixelFormat(deviceContext, &pfd);
        if (pixelFormat == 0) {
            throw std::runtime_error("ChoosePixelFormat failed");
        }

        if (!SetPixelFormat(deviceContext, pixelFormat, &pfd)) {
            throw std::runtime_error("SetPixelFormat failed");
        }

        renderingContext = wglCreateContext(deviceContext);
        if (renderingContext == nullptr) {
            throw std::runtime_error("wglCreateContext failed");
        }

        if (!wglMakeCurrent(deviceContext, renderingContext)) {
            throw std::runtime_error("wglMakeCurrent failed");
        }
    }

    void swapBuffers() {
        if (deviceContext) {
            SwapBuffers(deviceContext);
        }
    }

    void destroy() {
        if (renderingContext) {
            wglMakeCurrent(nullptr, nullptr);
            wglDeleteContext(renderingContext);
            renderingContext = nullptr;
        }
        deviceContext = nullptr;
    }
};