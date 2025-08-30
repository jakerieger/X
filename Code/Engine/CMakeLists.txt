set(ENGINE_SOURCES
    Core/Application.cpp
    Core/Application.hpp
    Core/Log.hpp
    Core/Log.cpp
    Core/Platform.hpp

    Math/Transform.hpp
    Math/Transform.cpp

    Render/Camera.cpp
    Render/Camera.hpp
    Render/Material.cpp
    Render/Material.hpp
    Render/Mesh.cpp
    Render/Mesh.hpp
    Render/RenderDevice.cpp
    Render/RenderDevice.hpp
    Render/Renderer.cpp
    Render/Renderer.hpp

    EnginePCH.h
    EnginePCH.cpp
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${ENGINE_SOURCES})

add_library(Engine STATIC ${ENGINE_SOURCES})

set_target_properties(Engine PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
    FOLDER "Engine"
)

target_precompile_headers(Engine PRIVATE EnginePCH.h)

target_include_directories(Engine
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/Core
    ${CMAKE_CURRENT_SOURCE_DIR}/Math
    ${CMAKE_CURRENT_SOURCE_DIR}/Render
)

# Core Diligent Engine libraries (always available)
target_link_libraries(Engine
    PUBLIC
    Diligent-GraphicsEngineD3D11-static
    Diligent-GraphicsEngineD3D12-static
    Diligent-GraphicsEngineVk-static
    Diligent-GraphicsEngineOpenGL-static
    Diligent-GraphicsTools

    glm::glm
    glfw
    spdlog::spdlog
)

# Conditional libraries based on what's available
if (TARGET Diligent-AssetLoader)
    target_link_libraries(Engine PUBLIC Diligent-AssetLoader)
    target_compile_definitions(Engine PUBLIC ENGINE_HAS_ASSET_LOADER)
endif ()

if (TARGET Diligent-TextureLoader)
    target_link_libraries(Engine PUBLIC Diligent-TextureLoader)
    target_compile_definitions(Engine PUBLIC ENGINE_HAS_TEXTURE_LOADER)
endif ()

# Alternative: STB for image loading
if (TARGET stb)
    target_link_libraries(Engine PUBLIC stb)
    target_compile_definitions(Engine PUBLIC ENGINE_HAS_STB)
endif ()

if (WIN32)
    target_link_libraries(Engine PRIVATE
        d3d11.lib
        dxgi.lib
        d3dcompiler.lib
    )
elseif (APPLE)
    find_library(METAL_FRAMEWORK Metal)
    find_library(METALKIT_FRAMEWORK MetalKit)
    target_link_libraries(Engine PRIVATE
        ${METAL_FRAMEWORK}
        ${METALKIT_FRAMEWORK}
    )
endif ()

# Compiler-specific options
#if (MSVC)
#    target_compile_options(Engine PRIVATE
#        /W4          # Warning level 4
#        /WX          # Warnings as errors
#        /permissive- # Conformance mode
#    )
#
#    # Disable specific MSVC warnings that are common with Diligent and third-party libraries
#    target_compile_options(Engine PRIVATE
#        /wd4250  # Inherits via dominance
#        /wd4251  # DLL interface warnings
#        /wd4996  # Deprecated function warnings (for spdlog/fmt compatibility)
#    )
#
#    # Silence the specific STL extension deprecation warnings
#    target_compile_definitions(Engine PRIVATE
#        _SILENCE_STDEXT_ARR_ITERS_DEPRECATION_WARNING
#        _SILENCE_ALL_MS_EXT_DEPRECATION_WARNINGS
#    )
#else ()
#    target_compile_options(Engine PRIVATE
#        -Wall
#        -Wextra
#        -Wpedantic
#        -Werror
#    )
#endif ()

target_compile_definitions(Engine
    PUBLIC
    $<$<CONFIG:Debug>:ENGINE_DEBUG>
    $<$<CONFIG:Release>:ENGINE_RELEASE>
    $<$<PLATFORM_ID:Windows>:ENGINE_PLATFORM_WINDOWS>
    $<$<PLATFORM_ID:Darwin>:ENGINE_PLATFORM_MACOS>
    $<$<PLATFORM_ID:Linux>:ENGINE_PLATFORM_LINUX>

    # Enable specific graphics APIs
    $<$<BOOL:${D3D11_SUPPORTED}>:ENGINE_D3D11_SUPPORTED>
    $<$<BOOL:${D3D12_SUPPORTED}>:ENGINE_D3D12_SUPPORTED>
    $<$<BOOL:${VULKAN_SUPPORTED}>:ENGINE_VULKAN_SUPPORTED>
    $<$<BOOL:${GL_SUPPORTED}>:ENGINE_GL_SUPPORTED>
    $<$<BOOL:${METAL_SUPPORTED}>:ENGINE_METAL_SUPPORTED>
)