#pragma once

#include <memory>

// Platform detection using macros
#ifdef _WIN32
	/* Windows x64/x86 */
	#ifdef _WIN64
		/* Window x64 */
		#define RB_PLATFORM_WINDOWS
	#else
		#error "x86 Builds are not supported"
	#endif
#elif defined(__APPLE__) || (__MACH__)
	#include <TargetConditionals.h>
	/* TARGET_OS_MAC exists on all the platforms
	 * so we must check all of them (in this order)
	 * to ensure that we're running on MAC
	 * and not some other Apple platform */
	#if TARGET_IPHONE_SIMULATOR == 1
		#error "IOS simulator is not supported!"
	#elif TARGET_OS_PHONE == 1
		#define RB_PLATFORM_IOS
		#error "IOS is not supported"
	#elif TARGET_OS_MAC == 1
		#define RB_PLATFORM_MACOS
		#error "MacOS is not supported (but it really should be)"
	#else
		#define RHOMBUS_API __declspec(dllimport)
		#error "Unknown Apple platform"
	#endif
/* We also have to check __ANDROID__ before __linux__
* since android is based on the linux kernel
* it has __linux__ defined */
#elif defined(__ANDROID__)
	#define RB_PLATFORM_ANDROID
	#error "Android is not supported"
#elif defined(__linux__)
	#define RB_PLATFORM_LINUX
	#error "Linux is not supported (but it really should be)"
#else
	/* Unknown compiler/platform */
	#error "Unknown platform!"
#endif // End of platform detection


// Code for using dll instead of static library
#ifdef RB_PLATFORM_WINDOWS
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
	#if defined(RB_PLATFORM_WINDOWS)
		#define RB_DEBUGBREAK() __debugbreak()
	#elif defined(RB_PLATFORM_LINUX)
		#include <signal.h>
		#define RB_DEBUGBREAK() raise(SIGTRAP)
	#else
		#error "Platform doesn't support debugbreak yet!"
	#endif
	#define RB_ENABLE_ASSERTS
#else
	#define RB_DEBUGBREAK()
#endif

#ifdef RB_ENABLE_ASSERTS
	#define RB_ASSERT(x, ...) { if(!(x)) { RB_ERROR("Assertion Failed: {0}", __VA_ARGS__); RB_DEBUGBREAK(); } }
	#define RB_CORE_ASSERT(x, ...) { if(!(x)) { RB_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); RB_DEBUGBREAK(); } }
#else
	#define RB_ASSERT(x, ...)
	#define RB_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)

#define RB_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...);}

namespace rhombus {

	template<typename T>
	using Scope = std::unique_ptr<T>;

	template<typename T>
	using Ref = std::shared_ptr<T>;
	  
}
