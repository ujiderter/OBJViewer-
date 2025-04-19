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

    // �������� ��������� OpenGL
    void create(HDC hdc) {
        deviceContext = hdc;

        // ��������� �������� ��� OpenGL
        PIXELFORMATDESCRIPTOR pfd = {
            sizeof(PIXELFORMATDESCRIPTOR),
            1,                              // ������
            PFD_DRAW_TO_WINDOW |           // ��������� ��������� � ����
            PFD_SUPPORT_OPENGL |           // ��������� OpenGL
            PFD_DOUBLEBUFFER,              // ������� �����������
            PFD_TYPE_RGBA,                 // RGBA ������
            32,                            // 32-������ ����
            0, 0, 0, 0, 0, 0,              // ���������� ���� �����
            0,                             // ��� �����-������
            0,                             // ���������� ��� ������
            0,                             // ��� ���������������� ������
            0, 0, 0, 0,                    // ���������� ���� �����������
            24,                            // 24-������ Z-�����
            8,                             // 8-������ ����� ���������
            0,                             // ��� ���������������� ������
            PFD_MAIN_PLANE,                // �������� ����
            0,                             // ���������������
            0, 0, 0                        // ���������� ����� ����
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