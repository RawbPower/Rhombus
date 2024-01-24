#pragma once

namespace rhombus
{
	class UUID
	{
	public:
		UUID();
		UUID(uint32_t uuid);
		UUID(const UUID&) = default;

		operator uint32_t() const{ return m_UUID; }

	private:
		uint32_t m_UUID;
	};
}

namespace std
{
	template <typename T> struct hash;

	template<>
	struct hash<rhombus::UUID>
	{
		std::size_t operator()(const rhombus::UUID& uuid) const
		{
			return (uint32_t)uuid;
		}
	};
}
