#ifndef OBJVIEWER_WINAPIRENDERER_H
#define OBJVIEWER_WINAPIRENDERER_H

#include <windows.h>
#include <commdlg.h>
#include <memory>
#include <limits>
#include <valarray>
#include "../model/obj/OBJModel.h"
#include "Renderer.h"
#include "../model/math/Vector3D.h"
#include "../controller/Triangle.h"
#include <gdiplus.h>
#include <numeric>

#pragma comment(lib, "gdiplus.lib")

#define ID_FILE_OPEN 1001

class WinAPIRenderer final : public Renderer {
private:
    ULONG_PTR gdiplusToken; // Токен для GDI+
    Gdiplus::GdiplusStartupInput gdiplusStartupInput;

    HWND hwnd{nullptr};
    HDC hdc{nullptr};
    std::vector<Triangle> model;
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

    static void drawFilledTriangle(HDC hdc, const Triangle::Vertex& v1, const Triangle::Vertex& v2, const Triangle::Vertex& v3, float intencity) {
        Gdiplus::Graphics graphics(hdc);
        Gdiplus::SolidBrush brush(Gdiplus::Color(255 * intencity, 255 * intencity, 255 * intencity));

        Gdiplus::PointF points[3] = {
                Gdiplus::PointF(static_cast<Gdiplus::REAL>(v1.x), static_cast<Gdiplus::REAL>(v1.y)),
                Gdiplus::PointF(static_cast<Gdiplus::REAL>(v2.x), static_cast<Gdiplus::REAL>(v2.y)),
                Gdiplus::PointF(static_cast<Gdiplus::REAL>(v3.x), static_cast<Gdiplus::REAL>(v3.y))
        };

        graphics.FillPolygon(&brush, points, 3);
    }
public:
    WinAPIRenderer() {
        Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr);
    }

    ~WinAPIRenderer() override {
        cleanup();
        Gdiplus::GdiplusShutdown(gdiplusToken);
    }

    WinAPIRenderer(const WinAPIRenderer&) = delete;
    WinAPIRenderer& operator=(const WinAPIRenderer&) = delete;

    WinAPIRenderer(WinAPIRenderer&&) noexcept = default;
    WinAPIRenderer& operator=(WinAPIRenderer&&) noexcept = default;

    void setEventHandler(IEventHandler* handler) override {
        eventHandler = handler;
    }

    [[nodiscard]] bool initialize() override { return true; }

    void render(const std::vector<Triangle>& triangles) override {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        // Получаем размеры окна
        RECT clientRect;
        GetClientRect(hwnd, &clientRect);
        int windowWidth = clientRect.right - clientRect.left;
        int windowHeight = clientRect.bottom - clientRect.top;

        // Создаем задний буфер
        HDC backBufferDC = CreateCompatibleDC(hdc); // Контекст устройства для заднего буфера
        HBITMAP backBufferBitmap = CreateCompatibleBitmap(hdc, windowWidth, windowHeight); // Битмап для заднего буфера
        HBITMAP oldBitmap = (HBITMAP)SelectObject(backBufferDC, backBufferBitmap); // Выбираем битмап в контекст

        // Очищаем задний буфер (заполняем черным цветом)
        HBRUSH hBrush = CreateSolidBrush(RGB(0, 0, 0)); // Черный цвет
        FillRect(backBufferDC, &clientRect, hBrush);
        DeleteObject(hBrush);

        // Рисуем треугольники в задний буфер
        Gdiplus::Graphics graphics(backBufferDC);

        std::vector<Triangle> processedTriangles;
        for (const auto& triangle : triangles) {
            Triangle t(triangle);
            t.scale(150.0f);
            t.translate(windowWidth / 2, windowHeight / 2);
            processedTriangles.push_back(t);
        }

        std::sort(processedTriangles.begin(), processedTriangles.end());

        VecMath::Vector3D<float> lightDirection(0.0f, 0.0f, 1.0f);

        for (const auto& triangle : processedTriangles) {
            if (!triangle.isVisible()) {
                continue; // Пропускаем невидимые грани
            }

            // Получаем вершины треугольника
            auto [v1, v2, v3] = triangle.getVertices();

            // Вычисляем интенсивность света
            float intensity = triangle.computeLightIntensity(lightDirection);

            // Отрисовываем треугольник
            drawFilledTriangle(backBufferDC, v1, v2, v3, intensity);
        }

        // Копируем задний буфер на экран
        BitBlt(hdc, 0, 0, windowWidth, windowHeight, backBufferDC, 0, 0, SRCCOPY);

        // Освобождаем ресурсы заднего буфера
        SelectObject(backBufferDC, oldBitmap);
        DeleteObject(backBufferBitmap);
        DeleteDC(backBufferDC);

        EndPaint(hwnd, &ps);
    }

    void cleanup() override {
        if (hdc) ReleaseDC(hwnd, hdc);
        hdc = nullptr;
    }

    void updateModel(std::vector<Triangle> md) override{
        model = md;
        if (!model.empty()) {
            InvalidateRect(hwnd, nullptr, TRUE); // Перерисовываем окно
        } else {
            MessageBox(hwnd, L"Failed to load OBJ model", L"Error", MB_ICONERROR | MB_OK);
        }
    }

    void run() override {
        WNDCLASS wc{};
        wc.lpfnWndProc = WndProc;
        wc.hInstance = GetModuleHandle(nullptr);
        wc.lpszClassName = L"OBJViewer";

        RegisterClass(&wc);

        hwnd = CreateWindowEx(
                0, L"OBJViewer", L"OBJ Viewer", WS_OVERLAPPEDWINDOW,
                CW_USEDEFAULT, CW_USEDEFAULT, 800, 600,
                nullptr, nullptr, GetModuleHandle(nullptr), this
        );

        ShowWindow(hwnd, SW_SHOW);

        // Создание меню
        auto hMenu = CreateMenu();
        auto hFileMenu = CreatePopupMenu();
        AppendMenu(hFileMenu, MF_STRING, ID_FILE_OPEN, L"Open OBJ File");
        AppendMenu(hMenu, MF_POPUP, reinterpret_cast<UINT_PTR>(hFileMenu), L"File");
        SetMenu(hwnd, hMenu);

        // Основной цикл обработки сообщений
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
            SetWindowLongPtr(
                    hwnd,
                    GWLP_USERDATA,
                    reinterpret_cast<LONG_PTR>(createStruct->lpCreateParams)
            );
        }

        auto* renderer = reinterpret_cast<WinAPIRenderer*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

        switch (msg) {
            case WM_PAINT:
                if (renderer && !renderer->model.empty()) {
                    renderer->render(renderer->model);
                }
                return 0;

            case WM_DESTROY:
                PostQuitMessage(0);
                return 0;

            case WM_COMMAND: {
                if (LOWORD(wParam) == ID_FILE_OPEN) {
                    if (auto filePath = renderer->openFileDialog(); !filePath.empty()) {
                        renderer->eventHandler->handleEvent(Event(EventType::FileOpen, std::move(filePath)));
                    }
                }
                return 0;
            }

            default:
                return DefWindowProc(hwnd, msg, wParam, lParam);
        }
    }
};

#endif //OBJVIEWER_WINAPIRENDERER_H
