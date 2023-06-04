#pragma once

#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace Rhombus {

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
#define RB_CORE_TRACE(...)    ::Rhombus::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define RB_CORE_INFO(...)     ::Rhombus::Log::GetCoreLogger()->info(__VA_ARGS__)
#define RB_CORE_WARN(...)     ::Rhombus::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define RB_CORE_ERROR(...)    ::Rhombus::Log::GetCoreLogger()->error(__VA_ARGS__)
#define RB_CORE_FATAL(...)    ::Rhombus::Log::GetCoreLogger()->critical(__VA_ARGS__)

// Client log macros
#define RB_TRACE(...)    ::Rhombus::Log::GetClientLogger()->trace(__VA_ARGS__)
#define RB_INFO(...)     ::Rhombus::Log::GetClientLogger()->info(__VA_ARGS__)
#define RB_WARN(...)     ::Rhombus::Log::GetClientLogger()->warn(__VA_ARGS__)
#define RB_ERROR(...)    ::Rhombus::Log::GetClientLogger()->error(__VA_ARGS__)
#define RB_FATAL(...)    ::Rhombus::Log::GetClientLogger()->critical(__VA_ARGS__)

/* Macros are used here instead of functions so that they can be
	stripped from the distribution built with
	// if dist build
	#define RB_CORE_INFO
	etc
*/



