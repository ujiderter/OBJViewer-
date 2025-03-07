#ifndef OBJVIEWER_WINAPIRENDERER_H
#define OBJVIEWER_WINAPIRENDERER_H

#include <windows.h>
#include <commdlg.h>
#include <memory>
#include "OBJModel.h"
#include "Renderer.h"

#define ID_FILE_OPEN 1001

class WinAPIRenderer : public Renderer {
private:
    HWND hwnd;
    HDC hdc;
    std::unique_ptr<OBJModel> model;
    std::string currentFilePath;

    std::string openFileDialog() {
        OPENFILENAME ofn;
        char fileName[MAX_PATH] = "";

        ZeroMemory(&ofn, sizeof(ofn));
        ofn.lStructSize = sizeof(ofn);
        ofn.hwndOwner = hwnd;
        ofn.lpstrFilter = "OBJ Files (*.obj)\0*.obj\0All Files (*.*)\0*.*\0";
        ofn.lpstrFile = fileName;
        ofn.nMaxFile = MAX_PATH;
        ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
        ofn.lpstrDefExt = "obj";

        if (GetOpenFileName(&ofn)) {
            return std::string(fileName);
        }
        return "";
    }

public:
    WinAPIRenderer() : hwnd(nullptr), hdc(nullptr), model(nullptr) {}

    ~WinAPIRenderer() override {
        cleanup();
    }

    bool initialize() override {
        return true;
    }

    void render(const OBJModel& model) override {
        PAINTSTRUCT ps;
        hdc = BeginPaint(hwnd, &ps);

        for (const auto& face : model.getFaces()) {
            for (auto [v1, v2, v3] : face.vertexIndices) {
                if (v1 && v2 && v3) {
                    const auto& vertex1 = model.getVertices()[*v1];
                    const auto& vertex2 = model.getVertices()[*v2];
                    const auto& vertex3 = model.getVertices()[*v3];

                    MoveToEx(hdc, static_cast<int>(vertex1.x), static_cast<int>(vertex1.y), nullptr);
                    LineTo(hdc, static_cast<int>(vertex2.x), static_cast<int>(vertex2.y));

                    MoveToEx(hdc, static_cast<int>(vertex2.x), static_cast<int>(vertex2.y), nullptr);
                    LineTo(hdc, static_cast<int>(vertex3.x), static_cast<int>(vertex3.y));

                    MoveToEx(hdc, static_cast<int>(vertex3.x), static_cast<int>(vertex3.y), nullptr);
                    LineTo(hdc, static_cast<int>(vertex1.x), static_cast<int>(vertex1.y));
                }
            }
        }

        EndPaint(hwnd, &ps);
    }

    void cleanup() override {
        if (hdc) ReleaseDC(hwnd, hdc);
    }

    void handleEvents() override {
        MSG msg;
        while (PeekMessage(&msg, hwnd, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);

            if (msg.message == WM_COMMAND) {
                if (LOWORD(msg.wParam) == ID_FILE_OPEN) {
                    std::string filePath = openFileDialog();
                    if (!filePath.empty()) {
                        currentFilePath = filePath;
                        model = std::make_unique<OBJModel>();
                        OBJModel::loadOBJ(currentFilePath);
                        InvalidateRect(hwnd, nullptr, TRUE);
                    }
                }
            }
        }
    }

    void run() override {
        WNDCLASS wc = {};
        wc.lpfnWndProc = WndProc;
        wc.hInstance = GetModuleHandle(nullptr);
        wc.lpszClassName = "OBJViewer";

        RegisterClass(&wc);

        hwnd = CreateWindow("OBJViewer", "OBJ Viewer", WS_OVERLAPPEDWINDOW,
                            CW_USEDEFAULT, CW_USEDEFAULT, 800, 600,
                            nullptr, nullptr, GetModuleHandle(nullptr), this);

        ShowWindow(hwnd, SW_SHOW);

        HMENU hMenu = CreateMenu();
        HMENU hFileMenu = CreatePopupMenu();
        AppendMenu(hFileMenu, MF_STRING, ID_FILE_OPEN, "Open OBJ File");
        AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hFileMenu, "File");
        SetMenu(hwnd, hMenu);

        MSG msg;
        while (GetMessage(&msg, nullptr, 0, 0)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

private:
    static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
        if (msg == WM_CREATE) {
            SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(reinterpret_cast<CREATESTRUCT*>(lParam)->lpCreateParams));
        }

        auto* renderer = reinterpret_cast<WinAPIRenderer*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

        switch (msg) {
            case WM_PAINT: {
                if (renderer && renderer->model) {
                    renderer->render(*renderer->model);
                }
                break;
            }

            case WM_DESTROY: {
                PostQuitMessage(0);
                break;
            }

            default:
                return DefWindowProc(hwnd, msg, wParam, lParam);
        }
        return 0;
    }
};

#endif //OBJVIEWER_WINAPIRENDERER_H
