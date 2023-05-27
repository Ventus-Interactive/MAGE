
#include <_common/sdebug.h>
#include "include.hpp"

void mod_window_init() {
    PrintD("Mod::window_init")
}

void Test_mod_window_init() {
    PrintD("Test::window_init")
}

namespace MAGE {
namespace Window {

    void Application::Init(const char* name, int width, int height) {
        PrintD("Window::Application::Init()")

        InitWindow(width, height, name);
        SetTargetFPS(60);
        DisableCursor();

        this->camera = { 0 };
        this->camera.position = (Vector3){ 0.0f, 2.0f, 4.0f };    // Camera position
        this->camera.target = (Vector3){ 0.0f, 2.0f, 0.0f };      // Camera looking at point
        this->camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
        this->camera.fovy = 60.0f;                                // Camera field-of-view Y
        this->camera.projection = CAMERA_PERSPECTIVE;             // Camera projection type
    }
    void Application::OnExit() {
        CloseWindow();
    }

}}

