// Author: Jake Rieger
// Created: 8/29/2025.
//

#include "SandboxApp.hpp"
#include "Core/Log.hpp"
#include "Render/Renderer.hpp"

namespace X {
    using namespace Core;
    using namespace Render;

    SandboxApp::SandboxApp() : Application({"Sandbox"}) {}

    void SandboxApp::Initialize() {
        Log::Info("Sandbox initialized");
        if (const auto renderer = GetRenderer()) { renderer->SetClearColor(1.0f, 0.0f, 0.0f, 1.0f); }
    }

    void SandboxApp::Update(f32 dT) {
        _time += dT;

        if (auto renderer = GetRenderer()) {
            float r = 0.2f + 0.1f * std::sin(_time * 0.5f);
            float g = 0.1f + 0.1f * std::sin(_time * 0.7f);
            float b = 0.3f + 0.1f * std::sin(_time * 0.3f);
            renderer->SetClearColor(r, g, b, 1.0f);
        }
    }

    void SandboxApp::Render() {
        //
    }

    void SandboxApp::Shutdown() {
        Log::Info("Shutting down");
    }

    void SandboxApp::OnWindowResize(u32 width, u32 height) {
        Log::Info("Window resized: {} x {}", width, height);
    }

    void SandboxApp::OnKeyPressed(i32 key, i32 action, i32 mods) {
        if (action == GLFW_PRESS) {
            Log::Info("Key pressed: {} (mods: {})", key, mods);

            if (key == GLFW_KEY_SPACE) { Log::Info("Space bar pressed!"); }
        }
    }

    void SandboxApp::OnMouseButton(int button, int action, int mods) {
        if (action == GLFW_PRESS) { Log::Info("Mouse button pressed: {}", button); }
    }

    void SandboxApp::OnMouseMove(double xpos, double ypos) {
        // Uncomment to see mouse movement (will be very verbose)
        // Log::Trace("Mouse moved to: ({:.1f}, {:.1f})", xpos, ypos);
    }

}  // namespace X