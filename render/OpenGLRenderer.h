#ifndef OBJVIEWER_OPENGLRENDERER_H
#define OBJVIEWER_OPENGLRENDERER_H

#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "Renderer.h"
#include "../model/math/Vector3D.h"
#include "../controller/Triangle.h"
#include "Light.h"
#include <memory>
#include <vector>

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")

#define ID_FILE_OPEN 1001

class OpenGLRenderer final : public Renderer {
private:
    HWND hwnd{nullptr};
    HDC hdc{nullptr};
    HGLRC hglrc{nullptr};
    std::vector<Triangle> model;
    std::unique_ptr<Light> light;
    std::string currentFilePath;

    [[nodiscard]] std::string openFileDialog() const {
        OPENFILENAME ofn{};
        wchar_t fileName[MAX_PATH] = L"";

        ZeroMemory(&ofn, sizeof(ofn));
        ofn.lStructSize = sizeof(ofn);
        ofn.hwndOwner = hwnd;
        ofn.lpstrFilter = L"OBJ Files (*.obj)\0*.obj\0All Files (*.*)\0*.*\0";
        ofn.lpstrFile = fileName;
        ofn.nMaxFile = MAX_PATH;
        ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
        ofn.lpstrDefExt = L"obj";

        if (GetOpenFileName(&ofn)) {
            int sizeNeeded = WideCharToMultiByte(CP_UTF8, 0, fileName, -1, nullptr, 0, nullptr, nullptr);
            std::string filePath(sizeNeeded, 0);
            WideCharToMultiByte(CP_UTF8, 0, fileName, -1, &filePath[0], sizeNeeded, nullptr, nullptr);
            return filePath;
        }
        return {};
    }

    bool setupPixelFormat() {
        PIXELFORMATDESCRIPTOR pfd = {
            sizeof(PIXELFORMATDESCRIPTOR),
            1,
            PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
            PFD_TYPE_RGBA,
            32,
            0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0,
            24,
            8,
            0,
            PFD_MAIN_PLANE,
            0, 0, 0, 0
        };

        int pixelFormat = ChoosePixelFormat(hdc, &pfd);
        if (!pixelFormat) return false;
        return SetPixelFormat(hdc, pixelFormat, &pfd);
    }

public:
    OpenGLRenderer()
        : light(std::make_unique<Light>(
            LightType::Directional,
            VecMath::Vector3D<float>(0.0f, 0.0f, 0.0f),
            VecMath::Vector3D<float>(0.0f, 0.0f, 1.0f),
            VecMath::Vector3D<float>(1.0f, 1.0f, 1.0f),
            1.0f // Full intensity
        )) {}

    ~OpenGLRenderer() override {
        cleanup();
    }

    OpenGLRenderer(const OpenGLRenderer&) = delete;
    OpenGLRenderer& operator=(const OpenGLRenderer&) = delete;

    OpenGLRenderer(OpenGLRenderer&&) noexcept = default;
    OpenGLRenderer& operator=(OpenGLRenderer&&) noexcept = default;

    void setCamera(std::shared_ptr<Camera> cam) override {
        camera = std::move(cam);
    }

    void setEventHandler(IEventHandler* handler) override {
        eventHandler = handler;
    }

    [[nodiscard]] bool initialize() override {
        hdc = GetDC(hwnd);
        if (!hdc) return false;

        if (!setupPixelFormat()) {
            ReleaseDC(hwnd, hdc);
            hdc = nullptr;
            return false;
        }

        hglrc = wglCreateContext(hdc);
        if (!hglrc) {
            ReleaseDC(hwnd, hdc);
            hdc = nullptr;
            return false;
        }

        if (!wglMakeCurrent(hdc, hglrc)) {
            wglDeleteContext(hglrc);
            ReleaseDC(hwnd, hdc);
            hglrc = nullptr;
            hdc = nullptr;
            return false;
        }

        glEnable(GL_DEPTH_TEST);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

        return true;
    }

    void render(const std::vector<Triangle>& triangles) override {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        RECT clientRect;
        GetClientRect(hwnd, &clientRect);
        glViewport(0, 0, clientRect.right - clientRect.left, clientRect.bottom - clientRect.top);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        if (camera) {
            auto proj = camera->getProjectionMatrix();
            glLoadMatrixf(&proj.m[0][0]);
        } else {
            gluPerspective(45.0f, static_cast<float>(clientRect.right) / clientRect.bottom, 0.1f, 100.0f);
        }

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        if (camera) {
            auto view = camera->getViewMatrix();
            glLoadMatrixf(&view.m[0][0]);
        } else {
            gluLookAt(0.0f, 0.0f, 5.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
        }

        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
        GLfloat lightPos[] = { light->getDirection().x, light->getDirection().y, light->getDirection().z, 0.0f };
        GLfloat lightCol[] = { light->getColor().x, light->getColor().y, light->getColor().z, 1.0f };
        glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, lightCol);
        glLightfv(GL_LIGHT0, GL_SPECULAR, lightCol);

        glBegin(GL_TRIANGLES);
        for (const auto& triangle : triangles) {
            if (!triangle.isVisible()) continue;
            float intensity = triangle.computeLightIntensity(light->getDirection());
            glColor3f(intensity, intensity, intensity);
            auto [v1, v2, v3] = triangle.getVertices();
            glVertex3f(v1.x, v1.y, v1.z);
            glVertex3f(v2.x, v2.y, v2.z);
            glVertex3f(v3.x, v3.y, v3.z);
        }
        glEnd();

        SwapBuffers(hdc);
    }

    void cleanup() override {
        if (hglrc) {
            wglMakeCurrent(nullptr, nullptr);
            wglDeleteContext(hglrc);
            hglrc = nullptr;
        }
        if (hdc) {
            ReleaseDC(hwnd, hdc);
            hdc = nullptr;
        }
    }

    void updateModel(std::vector<Triangle> md) override {
        model = md;
        if (!model.empty()) {
            InvalidateRect(hwnd, nullptr, TRUE);
        } else {
            MessageBox(hwnd, L"Failed to load OBJ model", L"Error", MB_ICONERROR | MB_OK);
        }
    }

    void run() override {
        WNDCLASS wc{};
        wc.lpfnWndProc = WndProc;
        wc.hInstance = GetModuleHandle(nullptr);
        wc.lpszClassName = L"OBJViewerOpenGL";

        RegisterClass(&wc);

        hwnd = CreateWindowEx(
            0, L"OBJViewerOpenGL", L"OBJ Viewer (OpenGL)", WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT, CW_USEDEFAULT, 800, 600,
            nullptr, nullptr, GetModuleHandle(nullptr), this
        );

        ShowWindow(hwnd, SW_SHOW);

        auto hMenu = CreateMenu();
        auto hFileMenu = CreatePopupMenu();
        AppendMenu(hFileMenu, MF_STRING, ID_FILE_OPEN, L"Open OBJ File");
        AppendMenu(hMenu, MF_POPUP, reinterpret_cast<UINT_PTR>(hFileMenu), L"File");
        SetMenu(hwnd, hMenu);

        if (!initialize()) {
            MessageBox(hwnd, L"Failed to initialize OpenGL", L"Error", MB_ICONERROR | MB_OK);
            DestroyWindow(hwnd);
            return;
        }

        MSG msg{};
        while (GetMessage(&msg, nullptr, 0, 0)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

private:
    static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
        if (msg == WM_CREATE) {
            auto* createStruct = reinterpret_cast<CREATESTRUCT*>(lParam);
            SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(createStruct->lpCreateParams));
        }

        auto* renderer = reinterpret_cast<OpenGLRenderer*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

        switch (msg) {
        case WM_PAINT:
            if (renderer && !renderer->model.empty()) {
                renderer->render(renderer->model);
            }
            return 0;

        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;

        case WM_COMMAND:
            if (LOWORD(wParam) == ID_FILE_OPEN) {
                if (auto filePath = renderer->openFileDialog(); !filePath.empty()) {
                    renderer->eventHandler->handleEvent(Event(EventType::FileOpen, std::move(filePath)));
                }
            }
            return 0;

        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
        }
    }
};

#endif // OBJVIEWER_OPENGLRENDERER_H
