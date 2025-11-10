#pragma once

#include <string_view>
#include <functional>

namespace rhombus::reflection
{
	template <typename T> constexpr std::string_view TypeName();

	template <>
	constexpr std::string_view TypeName<void>()
	{
		return "void";
	}

	//using type_name_prober = void;
	
	template <typename T>
	constexpr std::string_view wrapped_type_name()
	{
#ifdef __clang__
		return __PRETTY_FUNCTION__;
#elif defined(__GNUC__)
		return __PRETTY_FUNCTION__;
#elif defined(_MSC_VER)
		return __FUNCSIG__;
#else
#error "Unsupported compiler"
#endif
	}

	constexpr std::size_t wrapped_type_name_prefix_length()
	{
		return wrapped_type_name<void>().find(TypeName<void>());
	}

	constexpr std::size_t wrapped_type_name_suffix_length()
	{
		return wrapped_type_name<void>().length()
			- wrapped_type_name_prefix_length()
			- TypeName<void>().length();
	}

	template <typename T>
	constexpr std::string_view TypeName()
	{
		constexpr auto wrapped_name = wrapped_type_name<T>();
		constexpr auto prefix_length = wrapped_type_name_prefix_length();
		constexpr auto suffix_length = wrapped_type_name_suffix_length();
		constexpr auto type_name_length = wrapped_name.length() - prefix_length - suffix_length;
		return wrapped_name.substr(prefix_length, type_name_length);
	}

	// Custom hasing (for consistency)
	constexpr uint64_t hash(std::string_view str)
	{
		std::uint64_t hash_value = 0xcbf29ce484222325ULL;
		constexpr std::uint64_t prime = 0x100000001b3ULL;
		for (char c : str)
		{
			hash_value ^= static_cast<std::uint64_t>(c);
			hash_value *= prime;
		}
		return hash_value;
	}

	// Type ID
	template <typename Type>
	constexpr uint64_t TypeID()
	{
		return hash(TypeName<Type>());
	}
}
