#pragma once
#include "Engine/Platform/Platform.h"

namespace Sce
{
    class Logger
    {
    public:
        inline static const std::string LoggerName = "Engine";
        API static std::shared_ptr<spdlog::logger> Get();

        API static std::shared_ptr<spdlog::logger> Setup(spdlog::sink_ptr inSink);
    };
}
