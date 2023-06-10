#pragma once

#include <memory>

#ifdef RB_PLATFORM_WINDOWS
// Code for using dll instead of static library
#if RB_DYNAMIC_LINK
	#ifdef RB_BUILD_DLL
		// Exports in dll file
		#define RB_API __declspec(dllexport)
	#else
		#define RB_API __declspec(dllimport)
	#endif
#else
	#define RB_API
#endif

#else 
	#error Game Engine only support Windows (for now)
#endif

#ifdef RB_DEBUG
	#define RB_ENABLE_ASSERTS
#endif

#ifdef RB_ENABLE_ASSERTS
	#define RB_ASSERT(x, ...) { if(!(x)) { RB_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define RB_CORE_ASSERT(x, ...) { if(!(x)) { RB_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define RB_ASSERT(x, ...)
	#define RB_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)

#define RB_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

namespace rhombus {

	template<typename T>
	using Scope = std::unique_ptr<T>;

	template<typename T>
	using Ref = std::shared_ptr<T>;
	  
}
