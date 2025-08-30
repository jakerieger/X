// Author: Jake Rieger
// Created: 8/30/2025.
//

#pragma once

#include <spdlog/spdlog.h>
#include <memory>

namespace X::Core {

    class Log {
    public:
        static void Initialize();
        static void Shutdown();

        template<typename... Args>
        static void Trace(const std::string& format, Args&&... args) {
            GetLogger()->trace(fmt::runtime(format), std::forward<Args>(args)...);
        }

        template<typename... Args>
        static void Debug(const std::string& format, Args&&... args) {
            GetLogger()->debug(fmt::runtime(format), std::forward<Args>(args)...);
        }

        template<typename... Args>
        static void Info(const std::string& format, Args&&... args) {
            GetLogger()->info(fmt::runtime(format), std::forward<Args>(args)...);
        }

        template<typename... Args>
        static void Warn(const std::string& format, Args&&... args) {
            GetLogger()->warn(fmt::runtime(format), std::forward<Args>(args)...);
        }

        template<typename... Args>
        static void Error(const std::string& format, Args&&... args) {
            GetLogger()->error(fmt::runtime(format), std::forward<Args>(args)...);
        }

        template<typename... Args>
        static void Critical(const std::string& format, Args&&... args) {
            GetLogger()->critical(fmt::runtime(format), std::forward<Args>(args)...);
        }

    private:
        static std::shared_ptr<spdlog::logger> GetLogger();
        static std::shared_ptr<spdlog::logger> _logger;
    };

}  // namespace X::Core