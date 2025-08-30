// Author: Jake Rieger
// Created: 8/30/2025.
//

#pragma once

#include "EnginePCH.h"
#include "RenderDevice.h"

namespace X::Render {

    class Renderer {
    public:
        Renderer();
        ~Renderer();

        bool Initialize(GLFWwindow* window, u32 width, u32 height);
        void Shutdown();

        void BeginFrame() const;
        void EndFrame() const;

        void SetClearColor(f32 r, f32 g, f32 b, f32 a);
        void OnWindowResize(u32 width, u32 height);

        shared_ptr<RenderDevice> _device;
        u32 _width {0};
        u32 _height {0};
        Vec4 _clearColor {0.1f, 0.1f, 0.2f, 1.0f};
    };

}  // namespace X::Render