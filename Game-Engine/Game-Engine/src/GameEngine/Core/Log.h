#pragma once

#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace ge {

	class GE_API Log
	{
	public:
		// Two consoles, one for client (for the engine - core) and one for app
		
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger;  }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	
	};
}

// Core log macros
#define GE_CORE_TRACE(...)    ::ge::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define GE_CORE_INFO(...)     ::ge::Log::GetCoreLogger()->info(__VA_ARGS__)
#define GE_CORE_WARN(...)     ::ge::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define GE_CORE_ERROR(...)    ::ge::Log::GetCoreLogger()->error(__VA_ARGS__)
#define GE_CORE_FATAL(...)    ::ge::Log::GetCoreLogger()->critical(__VA_ARGS__)

// Client log macros
#define GE_TRACE(...)    ::ge::Log::GetClientLogger()->trace(__VA_ARGS__)
#define GE_INFO(...)     ::ge::Log::GetClientLogger()->info(__VA_ARGS__)
#define GE_WARN(...)     ::ge::Log::GetClientLogger()->warn(__VA_ARGS__)
#define GE_ERROR(...)    ::ge::Log::GetClientLogger()->error(__VA_ARGS__)
#define GE_FATAL(...)    ::ge::Log::GetClientLogger()->critical(__VA_ARGS__)

/* Macros are used here instead of functions so that they can be
	stripped from the distribution built with
	// if dist build
	#define GE_CORE_INFO
	etc
*/



