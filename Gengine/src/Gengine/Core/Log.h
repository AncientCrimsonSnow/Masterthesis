#pragma once

#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog//fmt/ostr.h"

namespace Gengine {
	class Log{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_Corelogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_Clientlogger; }
	private:
		static std::shared_ptr<spdlog::logger> s_Corelogger;
		static std::shared_ptr<spdlog::logger> s_Clientlogger;
	};
}

//Core Log macros
#define GG_CORE_FATAL(...)		::Gengine::Log::GetCoreLogger()->critical(__VA_ARGS__)
#define GG_CORE_ERROR(...)		::Gengine::Log::GetCoreLogger()->error(__VA_ARGS__)
#define GG_CORE_WARN(...)		::Gengine::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define GG_CORE_INFO(...)		::Gengine::Log::GetCoreLogger()->info(__VA_ARGS__)
#define GG_CORE_TRACE(...)		::Gengine::Log::GetCoreLogger()->trace(__VA_ARGS__)

//Client Log macros
#define GG_FATAL(...)			::Gengine::Log::GetClientLogger()->critical(__VA_ARGS__)
#define GG_ERROR(...)			::Gengine::Log::GetClientLogger()->error(__VA_ARGS__)
#define GG_WARN(...)			::Gengine::Log::GetClientLogger()->warn(__VA_ARGS__)
#define GG_INFO(...)			::Gengine::Log::GetClientLogger()->info(__VA_ARGS__)
#define GG_TRACE(...)			::Gengine::Log::GetClientLogger()->trace(__VA_ARGS__)