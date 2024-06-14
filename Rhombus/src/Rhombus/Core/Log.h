#pragma once

#include "Core.h"
#include "Rhombus/Math/Vector.h"
#include "Rhombus/Math/Matrix.h"
#include "Rhombus/Math/Quat.h"

// This ignores all warnings raised inside External headers
#pragma warning(push, 0)
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

template<typename OStream>
inline OStream& operator<<(OStream& os, const rhombus::Vec2& vector)
{
	return os << "(" << vector.x << "," << vector.y << ")";
}

template<typename OStream>
inline OStream& operator<<(OStream& os, const rhombus::Vec3& vector)
{
	return os << "(" << vector.x << "," << vector.y << "," << vector.z << ")";
}

template<typename OStream>
inline OStream& operator<<(OStream& os, const rhombus::Vec4& vector)
{
	return os << "(" << vector.x << "," << vector.y << "," << vector.z << "," << vector.w << ")";
}

template<typename OStream>
inline OStream& operator<<(OStream& os, const rhombus::Mat2& matrix)
{
	return os << "(" << matrix.cols[0][0] << "," << matrix.cols[0][1] << ")," << 
				"(" << matrix.cols[1][0] << "," << matrix.cols[1][1] << ")";
}

template<typename OStream>
inline OStream& operator<<(OStream& os, const rhombus::Mat3& matrix)
{
	return os << "(" << matrix.cols[0][0] << "," << matrix.cols[0][1] << "," << matrix.cols[0][2] << ")," <<
				"(" << matrix.cols[1][0] << "," << matrix.cols[1][1] << "," << matrix.cols[1][2] << ")," <<
				"(" << matrix.cols[2][0] << "," << matrix.cols[2][1] << "," << matrix.cols[2][2] << ")";
}

template<typename OStream>
inline OStream& operator<<(OStream& os, const rhombus::Mat4& matrix)
{
	return os << "(" << matrix.cols[0][0] << "," << matrix.cols[0][1] << "," << matrix.cols[0][2] << "," << matrix.cols[0][3] << ")," <<
		"(" << matrix.cols[1][0] << "," << matrix.cols[1][1] << "," << matrix.cols[1][2] << "," << matrix.cols[1][3] << ")," <<
		"(" << matrix.cols[2][0] << "," << matrix.cols[2][1] << "," << matrix.cols[2][2] << "," << matrix.cols[2][3] << ")," <<
		"(" << matrix.cols[3][0] << "," << matrix.cols[3][1] << "," << matrix.cols[3][2] << "," << matrix.cols[3][3] << ")";
}

template<typename OStream>
inline OStream& operator<<(OStream& os, const rhombus::Quat& quaternion)
{
	return os << "(" << quaternion.w << "{" << quaternion.x << "," << quaternion.y << "," << quaternion.z << "})";
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



