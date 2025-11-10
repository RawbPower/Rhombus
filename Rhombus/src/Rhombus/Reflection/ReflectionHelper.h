#pragma once

#include "rbpch.h"

namespace rhombus::reflection
{
	inline const void* VoidOffset(const void* data, size_t offset)
	{
		return static_cast<const uint8_t*>(data) + offset;
	}

	inline void* VoidOffset(void* data, size_t offset)
	{
		return static_cast<uint8_t*>(data) + offset;
	}

	template<typename T>
	void DrawPropertiesImGUI(T& object)
	{
		std::cout << "Test";
	}
}
