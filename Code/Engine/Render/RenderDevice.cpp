// Author: Jake Rieger
// Created: 8/30/2025.
//

#include "RenderDevice.hpp"
#include "Core/Log.hpp"

namespace X::Render {
    using namespace X::Core;

    RenderDevice::RenderDevice() {
        Log::Debug("RenderDevice created");
    }

    RenderDevice::~RenderDevice() {
        Shutdown();
        Log::Debug("RenderDevice destroyed");
    }

    bool RenderDevice::Initialize(GLFWwindow* window, uint32_t width, uint32_t height, GraphicsAPI api) {
        Log::Info("Initializing RenderDevice");

        _width  = width;
        _height = height;

        // Select API if auto
        if (api == GraphicsAPI::Auto) { api = SelectBestAPI(); }
        _currentAPI = api;

        Log::Info("Selected graphics API: {}", GetAPIName());

        // Initialize based on selected API
        bool success = false;
        switch (api) {
#if defined(ENGINE_D3D11_SUPPORTED)
            case GraphicsAPI::D3D11:
                success = InitializeD3D11(window, width, height);
                break;
#endif
#if defined(ENGINE_D3D12_SUPPORTED)
            case GraphicsAPI::D3D12:
                success = InitializeD3D12(window, width, height);
                break;
#endif
#if defined(ENGINE_VULKAN_SUPPORTED)
            case GraphicsAPI::Vulkan:
                success = InitializeVulkan(window, width, height);
                break;
#endif
#if defined(ENGINE_GL_SUPPORTED)
            case GraphicsAPI::OpenGL:
                success = InitializeOpenGL(window, width, height);
                break;
#endif
#if defined(ENGINE_METAL_SUPPORTED)
            case GraphicsAPI::Metal:
                success = InitializeMetal(window, width, height);
                break;
#endif
            default:
                Log::Error("Graphics API not supported on this platform");
                return false;
        }

        if (!success) {
            Log::Error("Failed to initialize graphics API");
            return false;
        }

        CreateBackBufferViews();

        Log::Info("RenderDevice initialized successfully with {}", GetAPIName());
        return true;
    }

    void RenderDevice::Shutdown() {
        _backBufferRTV.Release();
        _depthBufferDSV.Release();
        _swapChain.Release();
        _immediateContext.Release();
        _device.Release();
    }

    void RenderDevice::Present() {
        if (_swapChain) { _swapChain->Present(); }
    }

    void RenderDevice::OnWindowResize(uint32_t width, uint32_t height) {
        if (!_swapChain || (width == 0 || height == 0)) return;

        Log::Debug("RenderDevice handling resize: {}x{}", width, height);

        _width  = width;
        _height = height;

        // Release old views
        _backBufferRTV.Release();
        _depthBufferDSV.Release();

        // Resize swap chain
        _swapChain->Resize(width, height);

        // Recreate views
        CreateBackBufferViews();
    }

    GraphicsAPI RenderDevice::SelectBestAPI() {
#if defined(ENGINE_PLATFORM_WINDOWS)
    // On Windows, prefer D3D11 for compatibility
    #if defined(ENGINE_D3D11_SUPPORTED)
        return GraphicsAPI::D3D11;
    #elif defined(ENGINE_D3D12_SUPPORTED)
        return GraphicsAPI::D3D12;
    #elif defined(ENGINE_VULKAN_SUPPORTED)
        return GraphicsAPI::Vulkan;
    #endif
#elif defined(ENGINE_PLATFORM_MACOS)
    // On macOS, prefer Metal
    #if defined(ENGINE_METAL_SUPPORTED)
        return GraphicsAPI::Metal;
    #elif defined(ENGINE_VULKAN_SUPPORTED)
        return GraphicsAPI::Vulkan;
    #endif
#elif defined(ENGINE_PLATFORM_LINUX)
    // On Linux, prefer Vulkan
    #if defined(ENGINE_VULKAN_SUPPORTED)
        return GraphicsAPI::Vulkan;
    #elif defined(ENGINE_GL_SUPPORTED)
        return GraphicsAPI::OpenGL;
    #endif
#endif

        Log::Error("No suitable graphics API found for this platform");
        return GraphicsAPI::Auto;
    }

    const char* RenderDevice::GetAPIName() const {
        switch (_currentAPI) {
            case GraphicsAPI::D3D11:
                return "Direct3D 11";
            case GraphicsAPI::D3D12:
                return "Direct3D 12";
            case GraphicsAPI::Vulkan:
                return "Vulkan";
            case GraphicsAPI::OpenGL:
                return "OpenGL";
            case GraphicsAPI::Metal:
                return "Metal";
            default:
                return "Unknown";
        }
    }

#if defined(ENGINE_D3D11_SUPPORTED)
    bool RenderDevice::InitializeD3D11(GLFWwindow* window, uint32_t width, uint32_t height) {
        auto* pFactoryD3D11 = Diligent::GetEngineFactoryD3D11();

        Diligent::EngineD3D11CreateInfo EngineCI;
        pFactoryD3D11->CreateDeviceAndContextsD3D11(EngineCI, &_device, &_immediateContext);

        if (!_device) {
            Log::Error("Failed to create D3D11 device");
            return false;
        }

    #if defined(ENGINE_PLATFORM_WINDOWS)
        HWND hWnd = glfwGetWin32Window(window);
        Diligent::Win32NativeWindow Window {hWnd};

        Diligent::SwapChainDesc SCDesc;
        SCDesc.Width             = width;
        SCDesc.Height            = height;
        SCDesc.ColorBufferFormat = Diligent::TEX_FORMAT_RGBA8_UNORM_SRGB;
        SCDesc.DepthBufferFormat = Diligent::TEX_FORMAT_D32_FLOAT;

        Diligent::FullScreenModeDesc FSDesc;
        FSDesc.Fullscreen = false;

        pFactoryD3D11->CreateSwapChainD3D11(_device, _immediateContext, SCDesc, FSDesc, Window, &_swapChain);
    #endif

        return _swapChain != nullptr;
    }
#else
    bool RenderDevice::InitializeD3D11(GLFWwindow* window, uint32_t width, uint32_t height) {
        Log::Error("D3D11 support not compiled in");
        return false;
    }
#endif
    bool RenderDevice::InitializeD3D12(GLFWwindow* window, uint32_t width, uint32_t height) {
        Log::Error("D3D12 initialization not implemented yet");
        return false;
    }

    bool RenderDevice::InitializeVulkan(GLFWwindow* window, uint32_t width, uint32_t height) {
        Log::Error("Vulkan initialization not implemented yet");
        return false;
    }

    bool RenderDevice::InitializeOpenGL(GLFWwindow* window, uint32_t width, uint32_t height) {
        Log::Error("OpenGL initialization not implemented yet");
        return false;
    }

    bool RenderDevice::InitializeMetal(GLFWwindow* window, uint32_t width, uint32_t height) {
        Log::Error("Metal initialization not implemented yet");
        return false;
    }

    void RenderDevice::CreateBackBufferViews() {
        if (!_swapChain) return;

        auto* pBackBuffer = _swapChain->GetCurrentBackBufferRTV();
        _backBufferRTV    = RefCntAutoPtr<ITextureView>(pBackBuffer, Diligent::IID_TextureView);

        auto* pDepthBuffer = _swapChain->GetDepthBufferDSV();
        _depthBufferDSV    = RefCntAutoPtr<ITextureView>(pDepthBuffer, Diligent::IID_TextureView);
    }
}  // namespace X::Render