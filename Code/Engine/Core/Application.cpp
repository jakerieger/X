// Author: Jake Rieger
// Created: 8/30/2025.
//

#include "Application.hpp"
#include "Log.hpp"
#include "Renderer.hpp"

namespace X::Core {
    Application* Application::_instance = nullptr;

    Application::Application(const ApplicationConfig& config) : _config(config) {
        if (_instance) {
            Log::Error("Application already exists");
            return;
        }
        _instance = this;

        Log::Info("Initializing Application: {}", _config.title);

        InitializeWindow();
        InitializeRenderer();
    }

    Application::~Application() {
        Log::Info("Shutting down Application");
        Shutdown();

        if (_renderer) { _renderer.reset(); }

        if (_window) { glfwDestroyWindow(_window); }

        glfwTerminate();
        _instance = nullptr;
    }

    void Application::Run() {
        Log::Info("Running Application");

        _running       = true;
        _lastFrameTime = CAST<f32>(glfwGetTime());

        Initialize();

        while (_running && !glfwWindowShouldClose(_window)) {
            f32 currentTime = CAST<f32>(glfwGetTime());
            f32 dT          = currentTime - _lastFrameTime;
            _lastFrameTime  = currentTime;

            glfwPollEvents();
            ProcessInput();
            Update(dT);

            if (_renderer) {
                _renderer->BeginFrame();
                Render();
                _renderer->EndFrame();
            }
        }

        Log::Info("Shutdown Application");
    }

    void Application::InitializeWindow() {
        Log::Info("Initializing GLFW window");

        if (!glfwInit()) {
            Log::Error("Failed to initialize GLFW");
            return;
        }

        glfwSetErrorCallback(GLFWErrorCallback);

        // Configure GLFW for Diligent Engine
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

        // Create window
        _window = glfwCreateWindow(_config.width,
                                   _config.height,
                                   _config.title.c_str(),
                                   _config.fullscreen ? glfwGetPrimaryMonitor() : nullptr,
                                   nullptr);

        if (!_window) {
            Log::Error("Failed to create GLFW window");
            glfwTerminate();
            return;
        }

        // Center window on screen if not fullscreen
        if (!_config.fullscreen) {
            const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
            int xpos                = (mode->width - _config.width) / 2;
            int ypos                = (mode->height - _config.height) / 2;
            glfwSetWindowPos(_window, xpos, ypos);
        }

        // Set callbacks
        glfwSetWindowUserPointer(_window, this);
        glfwSetWindowCloseCallback(_window, GLFWWindowCloseCallback);
        glfwSetWindowSizeCallback(_window, GLFWWindowSizeCallback);
        glfwSetKeyCallback(_window, GLFWKeyCallback);
        glfwSetMouseButtonCallback(_window, GLFWMouseButtonCallback);
        glfwSetCursorPosCallback(_window, GLFWCursorPosCallback);

        Log::Info("Window created: {}x{}", _config.width, _config.height);
    }

    void Application::InitializeRenderer() {
        Log::Info("Initializing renderer");
        _renderer = std::make_shared<Render::Renderer>();

        if (!_renderer->Initialize(_window, _config.width, _config.height)) {
            Log::Error("Failed to initialize renderer");
            _renderer.reset();
        }
    }

    void Application::ProcessInput() {
        // Handle ESC to close
        if (glfwGetKey(_window, GLFW_KEY_ESCAPE) == GLFW_PRESS) { Close(); }
    }

    // GLFW Callbacks
    void Application::GLFWErrorCallback(int error, const char* description) {
        Log::Error("GLFW Error {}: {}", error, description);
    }

    void Application::GLFWWindowCloseCallback(GLFWwindow* window) {
        Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
        if (app) { app->Close(); }
    }

    void Application::GLFWWindowSizeCallback(GLFWwindow* window, int width, int height) {
        Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
        if (app) {
            app->_config.width  = static_cast<uint32_t>(width);
            app->_config.height = static_cast<uint32_t>(height);

            if (app->_renderer) { app->_renderer->OnWindowResize(width, height); }

            app->OnWindowResize(width, height);
        }
    }

    void Application::GLFWKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
        Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
        if (app) { app->OnKeyPressed(key, action, mods); }
    }

    void Application::GLFWMouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
        Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
        if (app) { app->OnMouseButton(button, action, mods); }
    }

    void Application::GLFWCursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
        Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
        if (app) { app->OnMouseMove(xpos, ypos); }
    }
}  // namespace X::Core