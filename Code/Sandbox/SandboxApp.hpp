// Author: Jake Rieger
// Created: 8/29/2025.
//

#pragma once

#include "Core/Application.hpp"

namespace X {

    class SandboxApp : public Core::Application {
    public:
        SandboxApp();
        virtual ~SandboxApp() = default;

        void Initialize() override;
        void Update(f32 dT) override;
        void Render() override;
        void Shutdown() override;

        void OnWindowResize(u32 width, u32 height) override;
        void OnKeyPressed(i32 key, i32 action, i32 mods) override;
        void OnMouseButton(i32 button, i32 action, i32 mods) override;
        void OnMouseMove(f64 x, f64 y) override;

    private:
        f32 _time {0.0f};
    };

}  // namespace X
