#include "rbpch.h"
#include "VariableID.h"


namespace std
{
	size_t hash<rhombus::VariableID>::operator()(const rhombus::VariableID& id) const noexcept
	{
		return static_cast<size_t>(id.GetHash());
	}
}

namespace rhombus
{
	bool operator==(const VariableID& lhs, const VariableID& rhs)
	{
		return lhs.m_type == rhs.m_type &&
			lhs.m_arraySize == rhs.m_arraySize &&
			lhs.m_pointerCount == rhs.m_pointerCount &&
			lhs.m_traitFlags == rhs.m_traitFlags;
	}
}