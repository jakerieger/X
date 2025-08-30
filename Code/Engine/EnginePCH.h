#pragma once

#include <memory>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <array>
#include <functional>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/fmt/fmt.h>

#include <RenderDevice.h>
#include <DeviceContext.h>
#include <SwapChain.h>
#include <MapHelper.hpp>
#include <GraphicsUtilities.h>

// Platform-specific includes
#if defined(ENGINE_PLATFORM_WINDOWS)
    #include <windows.h>
    #if defined(ENGINE_D3D11_SUPPORTED)
        #include "EngineFactoryD3D11.h"
    #endif
    #if defined(ENGINE_D3D12_SUPPORTED)
        #include "EngineFactoryD3D12.h"
    #endif
#endif

#if defined(ENGINE_VULKAN_SUPPORTED)
    #include "EngineFactoryVk.h"
#endif

#if defined(ENGINE_GL_SUPPORTED)
    #include "EngineFactoryOpenGL.h"
#endif

#if defined(ENGINE_PLATFORM_MACOS) && defined(ENGINE_METAL_SUPPORTED)
    #include "EngineFactoryMtl.h"
#endif

#include <GLFW/glfw3.h>

#if defined(ENGINE_PLATFORM_WINDOWS)
    #define GLFW_EXPOSE_NATIVE_WIN32
#elif defined(ENGINE_PLATFORM_LINUX)
    #define GLFW_EXPOSE_NATIVE_X11
#elif defined(ENGINE_PLATFORM_MACOS)
    #define GLFW_EXPOSE_NATIVE_COCOA
#endif
#include <GLFW/glfw3native.h>

#define CAST static_cast
#define CCAST const_cast
#define DCAST dynamic_cast
#define RCAST reinterpret_cast

namespace X {
    using u8   = uint8_t;
    using u16  = uint16_t;
    using u32  = uint32_t;
    using u64  = uint64_t;
    using uptr = uintptr_t;

    using i8   = int8_t;
    using i16  = int16_t;
    using i32  = int32_t;
    using i64  = int64_t;
    using iptr = intptr_t;

#if defined(__GNUC__) || defined(__clang__)
    using u128 = __uint128_t;
    using i128 = __int128_t;
#endif

    using f32 = float;
    using f64 = double;

    using cstr    = const char*;
    using str     = std::string;
    using wstr    = std::wstring;
    using strview = std::string_view;

    // Most used STL objects included for convenience
    using std::array;
    using std::make_shared;
    using std::make_unique;
    using std::optional;
    using std::shared_ptr;
    using std::unique_ptr;
    using std::unordered_map;
    using std::vector;
    using std::weak_ptr;

    namespace Core {
        class Application;
    }

    namespace Render {
        class Renderer;
        class RenderDevice;
        class Mesh;
        class Material;
        class Camera;
    }  // namespace Render

    using Diligent::IBuffer;
    using Diligent::IBufferView;
    using Diligent::IDeviceContext;
    using Diligent::IPipelineState;
    using Diligent::IRenderDevice;
    using Diligent::IShader;
    using Diligent::ISwapChain;
    using Diligent::ITexture;
    using Diligent::ITextureView;
    using Diligent::RefCntAutoPtr;

    using Vec2 = glm::vec2;
    using Vec3 = glm::vec3;
    using Vec4 = glm::vec4;
    using Mat3 = glm::mat3;
    using Mat4 = glm::mat4;
    using Quat = glm::quat;
}  // namespace X