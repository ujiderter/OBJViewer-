#include "Renderer.h"
#include "WinAPIRenderer.h"

int main() {
    std::unique_ptr<Renderer> renderer = std::make_unique<WinAPIRenderer>();
    renderer->run();
}