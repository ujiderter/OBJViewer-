#include "Renderer.h"
#include "WinAPIRenderer.h"

int main() {
    try {
        auto renderer = std::make_unique<WinAPIRenderer>();

        if (!renderer->initialize()) {
            std::cerr << "Failed to initialize renderer\n";
            return 1;
        }

        renderer->run();
        return 0;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    catch (...) {
        std::cerr << "Unknown error occurred\n";
        return 2;
    }
}