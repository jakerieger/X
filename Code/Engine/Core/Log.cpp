// Author: Jake Rieger
// Created: 8/30/2025.
//

#include "EnginePCH.h"
#include "Log.hpp"
#include <spdlog/sinks/stdout_color_sinks.h>

namespace X::Core {
    shared_ptr<spdlog::logger> Log::_logger;

    void Log::Initialize() {
        // Create console sink with color support
        auto consoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        consoleSink->set_pattern("[%T] [%^%l%$] %n: %v");

        // Create logger
        _logger = std::make_shared<spdlog::logger>("ENGINE", consoleSink);
        _logger->set_level(spdlog::level::trace);
        _logger->flush_on(spdlog::level::trace);

        // Register as default logger
        spdlog::register_logger(_logger);
        spdlog::set_default_logger(_logger);

        Info("Logging system initialized");
    }

    void Log::Shutdown() {
        if (_logger) {
            Info("Shutting down logging system");
            _logger->flush();
            spdlog::drop_all();
            _logger.reset();
        }
    }

    shared_ptr<spdlog::logger> Log::GetLogger() {
        if (!_logger) { Initialize(); }
        return _logger;
    }
}  // namespace X::Core