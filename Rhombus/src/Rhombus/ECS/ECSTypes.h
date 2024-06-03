#pragma once

#include <bitset>

namespace rhombus
{
	// Is this still red?
	using EntityID = uint32_t;
	const EntityID MAX_ENTITIES = 5000;

	using ComponentType = uint8_t;
	const ComponentType MAX_COMPONENTS = 64;

	// A bitset representing the relevant component to this entity or system
	using Signature = std::bitset<MAX_COMPONENTS>;
}
