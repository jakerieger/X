#include <iostream>

#include "SandboxApp.hpp"
#include "Core/Application.hpp"
#include "Core/Log.hpp"

namespace X::Core {
    unique_ptr<Application> CreateApplication() {
        return make_unique<SandboxApp>();
    }
}  // namespace X::Core

int main() {
    using namespace X::Core;

    Log::Initialize();
    Log::Info("Starting sandbox");

    try {
        const auto app = CreateApplication();
        if (app) {
            app->Run();
        } else {
            Log::Error("Failed to create application");
            return -1;
        }
    } catch (const std::exception& e) {
        Log::Critical(e.what());
        Log::Shutdown();
        return -1;
    }

    Log::Info("Finished sandbox");
    Log::Shutdown();

    return 0;
}