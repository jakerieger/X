// Author: Jake Rieger
// Created: 8/30/2025.
//

#pragma once

#include "EnginePCH.h"

namespace X::Core {

    struct ApplicationConfig {
        str title {"X"};
        u32 width {800};
        u32 height {600};
        bool vsync {true};
        bool fullscreen = {false};
    };

    class Application {
    public:
        explicit Application(const ApplicationConfig& config = {});
        virtual ~Application();

        void Run();

        virtual void Initialize() {}
        virtual void Update(f32 dT) {}
        virtual void Render() {}
        virtual void Shutdown() {}

        virtual void OnWindowResize(u32 width, u32 height) {}
        virtual void OnKeyPressed(i32 key, i32 action, i32 mods) {}
        virtual void OnMouseButton(i32 button, i32 action, i32 mods) {}
        virtual void OnMouseMove(f64 x, f64 y) {}

        GLFWwindow* GetWindow() const {
            return _window;
        }

        shared_ptr<Render::Renderer> GetRenderer() const {
            return _renderer;
        }

        bool IsRunning() const {
            return _running;
        }

        void Close() {
            _running = false;
        }

        static Application* GetInstance() {
            return _instance;
        }

    private:
        void InitializeWindow();
        void InitializeRenderer();
        void ProcessInput();

        static void GLFWErrorCallback(i32 error, const char* description);
        static void GLFWWindowCloseCallback(GLFWwindow* window);
        static void GLFWWindowSizeCallback(GLFWwindow* window, i32 width, i32 height);
        static void GLFWKeyCallback(GLFWwindow* window, i32 key, i32 scancode, i32 action, i32 mods);
        static void GLFWMouseButtonCallback(GLFWwindow* window, i32 button, i32 action, i32 mods);
        static void GLFWCursorPosCallback(GLFWwindow* window, f64 xpos, f64 ypos);

    private:
        ApplicationConfig _config;
        GLFWwindow* _window {nullptr};
        shared_ptr<Render::Renderer> _renderer {nullptr};
        bool _running {false};

        f32 _lastFrameTime {0.0f};

        static Application* _instance;
    };

    unique_ptr<Application> CreateApplication();
}  // namespace X::Core