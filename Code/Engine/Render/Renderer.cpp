// Author: Jake Rieger
// Created: 8/30/2025.
//

#include "Renderer.hpp"

#include "EnginePCH.h"
#include "RenderDevice.hpp"
#include "Core/Log.hpp"

namespace X::Render {
    using namespace X::Core;

    Renderer::Renderer() {
        Log::Debug("Renderer created");
    }

    Renderer::~Renderer() {
        Shutdown();
        Log::Debug("Renderer destroyed");
    }

    bool Renderer::Initialize(GLFWwindow* window, u32 width, u32 height) {
        Log::Info("Initializing Renderer");

        _width  = width;
        _height = height;

        _device = make_shared<RenderDevice>();
        if (!_device->Initialize(window, width, height)) {
            Log::Error("Failed to initialize render device");
            return false;
        }

        Log::Info("Initialized render device");
        return true;
    }

    void Renderer::Shutdown() {
        if (_device) {
            _device->Shutdown();
            _device.reset();
        }
    }

    void Renderer::BeginFrame() const {
        if (!_device) return;

        const float clearColor[] = {_clearColor.r, _clearColor.g, _clearColor.b, _clearColor.a};
        auto* context            = _device->GetImmediateContext();
        auto* rtv                = _device->GetBackBufferRTV();
        auto* dsv                = _device->GetDepthBufferDSV();

        context->SetRenderTargets(1, &rtv, dsv, Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

        context->ClearRenderTarget(rtv, clearColor, Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
        context->ClearDepthStencil(dsv,
                                   Diligent::CLEAR_DEPTH_FLAG,
                                   1.0f,
                                   0,
                                   Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
    }

    void Renderer::EndFrame() const {
        if (!_device) return;
        _device->Present();
    }

    void Renderer::SetClearColor(f32 r, f32 g, f32 b, f32 a) {
        _clearColor = {r, g, b, a};
    }

    void Renderer::OnWindowResize(u32 width, u32 height) {
        if (!_device || (width == 0 || height == 0)) return;

        Log::Info("Renderer handling window resize: {} x {}", width, height);

        _width  = width;
        _height = height;

        _device->OnWindowResize(width, height);
    }
}  // namespace X::Render