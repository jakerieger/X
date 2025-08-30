// Author: Jake Rieger
// Created: 8/30/2025.
//

#pragma once

#include "EnginePCH.h"

namespace X::Render {

    enum class GraphicsAPI { Auto, D3D11, D3D12, Vulkan, OpenGL, Metal };

    class RenderDevice {
    public:
        RenderDevice();
        ~RenderDevice();

        bool Initialize(GLFWwindow* window, uint32_t width, uint32_t height, GraphicsAPI api = GraphicsAPI::Auto);
        void Shutdown();

        void Present();
        void OnWindowResize(uint32_t width, uint32_t height);

        // Accessors
        IRenderDevice* GetDevice() const {
            return _device;
        }
        IDeviceContext* GetImmediateContext() const {
            return _immediateContext;
        }
        ISwapChain* GetSwapChain() const {
            return _swapChain;
        }
        ITextureView* GetBackBufferRTV() const {
            return _backBufferRTV;
        }
        ITextureView* GetDepthBufferDSV() const {
            return _depthBufferDSV;
        }

        GraphicsAPI GetAPI() const {
            return _currentAPI;
        }
        const char* GetAPIName() const;

    private:
        bool InitializeD3D11(GLFWwindow* window, uint32_t width, uint32_t height);
        bool InitializeD3D12(GLFWwindow* window, uint32_t width, uint32_t height);
        bool InitializeVulkan(GLFWwindow* window, uint32_t width, uint32_t height);
        bool InitializeOpenGL(GLFWwindow* window, uint32_t width, uint32_t height);
        bool InitializeMetal(GLFWwindow* window, uint32_t width, uint32_t height);

        void CreateBackBufferViews();
        GraphicsAPI SelectBestAPI();

    private:
        RefCntAutoPtr<IRenderDevice> _device;
        RefCntAutoPtr<IDeviceContext> _immediateContext;
        RefCntAutoPtr<ISwapChain> _swapChain;
        RefCntAutoPtr<ITextureView> _backBufferRTV;
        RefCntAutoPtr<ITextureView> _depthBufferDSV;

        GraphicsAPI _currentAPI = GraphicsAPI::Auto;
        uint32_t _width         = 0;
        uint32_t _height        = 0;
    };

}  // namespace X::Render