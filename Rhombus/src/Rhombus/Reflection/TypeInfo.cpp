#include "rbpch.h"
#include "TypeInfo.h"

namespace std
{
	size_t hash<rhombus::TypeID>::operator()(const rhombus::TypeID& id) const noexcept
	{
		return static_cast<size_t>(id.GetID());
	}
}