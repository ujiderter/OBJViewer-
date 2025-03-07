#ifndef OBJVIEWER_WINAPIRENDERER_H
#define OBJVIEWER_WINAPIRENDERER_H

#include <windows.h>
#include <commdlg.h>
#include <memory>
#include "OBJModel.h"
#include "Renderer.h"

#define ID_FILE_OPEN 1001

class WinAPIRenderer final : public Renderer {
private:
    HWND hwnd{nullptr};
    HDC hdc{nullptr};
    std::unique_ptr<OBJModel> model;
    std::string currentFilePath;

    [[nodiscard]] std::string openFileDialog() const {
        OPENFILENAME ofn{};
        char fileName[MAX_PATH] = "";

        ofn.lStructSize = sizeof(ofn);
        ofn.hwndOwner = hwnd;
        ofn.lpstrFilter = "OBJ Files (*.obj)\0*.obj\0All Files (*.*)\0*.*\0";
        ofn.lpstrFile = fileName;
        ofn.nMaxFile = MAX_PATH;
        ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
        ofn.lpstrDefExt = "obj";

        return GetOpenFileName(&ofn) ? std::string(fileName) : std::string{};
    }

public:
    WinAPIRenderer() = default;
    ~WinAPIRenderer() override { cleanup(); }

    WinAPIRenderer(const WinAPIRenderer&) = delete;
    WinAPIRenderer& operator=(const WinAPIRenderer&) = delete;

    WinAPIRenderer(WinAPIRenderer&&) noexcept = default;
    WinAPIRenderer& operator=(WinAPIRenderer&&) noexcept = default;

    [[nodiscard]] bool initialize() override { return true; }

    void render(const OBJModel& modelToRender) override {
        PAINTSTRUCT ps;
        hdc = BeginPaint(hwnd, &ps);

        for (const auto& face : modelToRender.getFaces()) {
            for (const auto& [v1, v2, v3] : face.vertexIndices) {
                if (v1 && v2 && v3) {
                    const auto& vertices = modelToRender.getVertices();
                    const auto& vertex1 = vertices[*v1];
                    const auto& vertex2 = vertices[*v2];
                    const auto& vertex3 = vertices[*v3];

                    auto drawLine = [this](int x1, int y1, int x2, int y2) {
                        MoveToEx(hdc, x1, y1, nullptr);
                        LineTo(hdc, x2, y2);
                    };

                    drawLine(static_cast<int>(vertex1.x), static_cast<int>(vertex1.y),
                             static_cast<int>(vertex2.x), static_cast<int>(vertex2.y));

                    drawLine(static_cast<int>(vertex2.x), static_cast<int>(vertex2.y),
                             static_cast<int>(vertex3.x), static_cast<int>(vertex3.y));

                    drawLine(static_cast<int>(vertex3.x), static_cast<int>(vertex3.y),
                             static_cast<int>(vertex1.x), static_cast<int>(vertex1.y));
                }
            }
        }

        EndPaint(hwnd, &ps);
    }

    void cleanup() override {
        if (hdc) ReleaseDC(hwnd, hdc);
        hdc = nullptr;
    }

    void handleEvents() override {
        MSG msg{};
        while (PeekMessage(&msg, hwnd, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);

            if (msg.message == WM_COMMAND && LOWORD(msg.wParam) == ID_FILE_OPEN) {
                if (auto filePath = openFileDialog(); !filePath.empty()) {
                    currentFilePath = std::move(filePath);
                    model = std::make_unique<OBJModel>();
                    if (OBJModel::loadOBJ(currentFilePath)) {
                        InvalidateRect(hwnd, nullptr, TRUE);
                    }
                }
            }
        }
    }

    void run() override {
        WNDCLASS wc{};
        wc.lpfnWndProc = WndProc;
        wc.hInstance = GetModuleHandle(nullptr);
        wc.lpszClassName = "OBJViewer";

        RegisterClass(&wc);

        hwnd = CreateWindowEx(
                0, "OBJViewer", "OBJ Viewer", WS_OVERLAPPEDWINDOW,
                CW_USEDEFAULT, CW_USEDEFAULT, 800, 600,
                nullptr, nullptr, GetModuleHandle(nullptr), this
        );

        ShowWindow(hwnd, SW_SHOW);

        // Create menu
        auto hMenu = CreateMenu();
        auto hFileMenu = CreatePopupMenu();
        AppendMenu(hFileMenu, MF_STRING, ID_FILE_OPEN, "Open OBJ File");
        AppendMenu(hMenu, MF_POPUP, reinterpret_cast<UINT_PTR>(hFileMenu), "File");
        SetMenu(hwnd, hMenu);

        // Message loop
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
                if (renderer && renderer->model) {
                    renderer->render(*renderer->model);
                }
                return 0;

            case WM_DESTROY:
                PostQuitMessage(0);
                return 0;

            default:
                return DefWindowProc(hwnd, msg, wParam, lParam);
        }
    }
};

#endif //OBJVIEWER_WINAPIRENDERER_H
