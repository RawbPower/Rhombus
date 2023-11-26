#pragma once

// This ignores all warnings raised inside External headers
#pragma warning(push, 0)
#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"
#pragma warning(pop)

namespace rhombus {

	class RB_API Log
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
#define RB_CORE_TRACE(...)    ::rhombus::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define RB_CORE_INFO(...)     ::rhombus::Log::GetCoreLogger()->info(__VA_ARGS__)
#define RB_CORE_WARN(...)     ::rhombus::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define RB_CORE_ERROR(...)    ::rhombus::Log::GetCoreLogger()->error(__VA_ARGS__)
#define RB_CORE_CRITICAL(...)    ::rhombus::Log::GetCoreLogger()->critical(__VA_ARGS__)

// Client log macros
#define RB_TRACE(...)    ::rhombus::Log::GetClientLogger()->trace(__VA_ARGS__)
#define RB_INFO(...)     ::rhombus::Log::GetClientLogger()->info(__VA_ARGS__)
#define RB_WARN(...)     ::rhombus::Log::GetClientLogger()->warn(__VA_ARGS__)
#define RB_ERROR(...)    ::rhombus::Log::GetClientLogger()->error(__VA_ARGS__)
#define RB_CRITICAL(...)    ::rhombus::Log::GetClientLogger()->critical(__VA_ARGS__)

/* Macros are used here instead of functions so that they can be
	stripped from the distribution built with
	// if dist build
	#define RB_CORE_INFO
	etc
*/



