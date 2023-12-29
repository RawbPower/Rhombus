#pragma once

#include <xhash>

namespace rhombus
{
	class UUID
	{
	public:
		UUID();
		UUID(uint64_t uuid);
		UUID(const UUID&) = default;

		operator uint64_t() const{ return m_UUID; }

	private:
		uint64_t m_UUID;
	};
}

namespace std
{
	template<>
	struct hash<rhombus::UUID>
	{
		std::size_t operator()(const rhombus::UUID& uuid)
		{
			return hash<uint64_t>()((uint64_t)uuid);
		}
	};
}
