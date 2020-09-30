#include "Engine/pch.h"

#include "Logger.h"

namespace Sce
{
    std::shared_ptr<spdlog::logger> Logger::Get()
    {
        auto logger = spdlog::get(LoggerName);
        if (!logger)
            throw std::runtime_error("use Logger::Setup");
        return logger;
    }


    std::shared_ptr<spdlog::logger> Logger::Setup(spdlog::sink_ptr inSink)
    {
        if (!spdlog::get(LoggerName))
        {
            auto logger = std::make_shared<spdlog::logger>(LoggerName, inSink);
            logger->set_level(spdlog::level::info);
            spdlog::register_logger(logger);
        }
        Get()->info("logger initialized");
        return spdlog::get(LoggerName);
    }
}
