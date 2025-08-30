set(CMAKE_WARN_DEPRECATED OFF)
set(CMAKE_SUPPRESS_DEVELOPER_WARNINGS ON)
set(CMAKE_POLICY_DEFAULT_CMP0000 NEW)

# Handle the specific policy issues
if (POLICY CMP0175)
    cmake_policy(SET CMP0175 NEW)
endif ()

set(FETCHCONTENT_QUIET ON)

# Diligent Engine - Core only (without problematic image loaders)
message(STATUS "Fetching Diligent Engine Core...")
FetchContent_Declare(
    DiligentCore
    GIT_REPOSITORY https://github.com/DiligentGraphics/DiligentCore.git
    GIT_TAG v2.5.5
    GIT_SHALLOW TRUE
)

# Configure Diligent Core options
set(DILIGENT_BUILD_SAMPLES OFF CACHE BOOL "Build Diligent samples")
set(DILIGENT_BUILD_DEMOS OFF CACHE BOOL "Build Diligent demos")
set(DILIGENT_BUILD_UNITY_PLUGIN OFF CACHE BOOL "Build Unity plugin")
set(DILIGENT_BUILD_TESTS OFF CACHE BOOL "Build Diligent tests")
set(DILIGENT_INSTALL_CORE OFF CACHE BOOL "Install Diligent Core")

# Enable/disable graphics APIs based on platform
if (PLATFORM_WIN32)
    set(D3D11_SUPPORTED ON CACHE BOOL "Enable D3D11")
    set(D3D12_SUPPORTED ON CACHE BOOL "Enable D3D12")
    set(VULKAN_SUPPORTED ON CACHE BOOL "Enable Vulkan")
elseif (PLATFORM_MACOS)
    set(METAL_SUPPORTED ON CACHE BOOL "Enable Metal")
    set(VULKAN_SUPPORTED ON CACHE BOOL "Enable Vulkan")
    set(D3D11_SUPPORTED OFF CACHE BOOL "Disable D3D11 on macOS")
    set(D3D12_SUPPORTED OFF CACHE BOOL "Disable D3D12 on macOS")
elseif (PLATFORM_LINUX)
    set(VULKAN_SUPPORTED ON CACHE BOOL "Enable Vulkan")
    set(GL_SUPPORTED ON CACHE BOOL "Enable OpenGL")
    set(D3D11_SUPPORTED OFF CACHE BOOL "Disable D3D11 on Linux")
    set(D3D12_SUPPORTED OFF CACHE BOOL "Disable D3D12 on Linux")
endif ()

FetchContent_MakeAvailable(DiligentCore)

# Conditionally fetch DiligentTools only if you need texture/asset loading
option(ENABLE_DILIGENT_TOOLS "Enable Diligent Tools (includes image loaders)" OFF)

if (ENABLE_DILIGENT_TOOLS)
    message(STATUS "Fetching Diligent Tools...")
    FetchContent_Declare(
        DiligentTools
        GIT_REPOSITORY https://github.com/DiligentGraphics/DiligentTools.git
        GIT_TAG v2.5.5
        GIT_SHALLOW TRUE
    )

    # Try to fix libpng issues
    set(DILIGENT_INSTALL_TOOLS OFF CACHE BOOL "Install Diligent Tools")
    set(PNG_BUILD_ZLIB ON CACHE BOOL "Custom zlib build")
    set(PNG_SHARED OFF CACHE BOOL "Build shared lib")
    set(PNG_STATIC ON CACHE BOOL "Build static lib")
    set(PNG_TESTS OFF CACHE BOOL "Build libpng tests")
    set(PNG_DEBUG OFF CACHE BOOL "Enable debug output")

    # Alternative: Use system libpng if available
    find_package(PNG QUIET)
    if (PNG_FOUND)
        set(PNG_USE_SYSTEM ON CACHE BOOL "Use system PNG")
    endif ()

    FetchContent_MakeAvailable(DiligentTools)
endif ()

# GLFW for windowing
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.4
    GIT_SHALLOW TRUE
)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "Build GLFW docs")
set(GLFW_BUILD_TESTS OFF CACHE BOOL "Build GLFW tests")
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "Build GLFW examples")
set(GLFW_INSTALL OFF CACHE BOOL "Generate GLFW installation target")

FetchContent_MakeAvailable(glfw)

# GLM for math
FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG 1.0.1
    GIT_SHALLOW TRUE
)

set(GLM_BUILD_TESTS OFF CACHE BOOL "Build GLM tests")
set(GLM_BUILD_INSTALL OFF CACHE BOOL "Install GLM")

FetchContent_MakeAvailable(glm)

# spdlog for logging
FetchContent_Declare(
    spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG v1.12.0
    GIT_SHALLOW TRUE
)

set(SPDLOG_BUILD_TESTS OFF CACHE BOOL "Build spdlog tests")
set(SPDLOG_BUILD_EXAMPLE OFF CACHE BOOL "Build spdlog examples")
set(SPDLOG_INSTALL OFF CACHE BOOL "Install spdlog")

FetchContent_MakeAvailable(spdlog)

# Alternative texture loading library (if you need image loading)
if (NOT ENABLE_DILIGENT_TOOLS)
    message(STATUS "Consider using stb_image as an alternative to Diligent's texture loader")

    # STB (single-file libraries) - much simpler alternative
    FetchContent_Declare(
        stb
        GIT_REPOSITORY https://github.com/nothings/stb.git
        GIT_TAG master
        GIT_SHALLOW TRUE
    )
    FetchContent_MakeAvailable(stb)

    # Create interface library for STB
    if (stb_POPULATED)
        add_library(stb INTERFACE)
        target_include_directories(stb INTERFACE ${stb_SOURCE_DIR})
    endif ()
endif ()