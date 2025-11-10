#pragma once

#include "rbpch.h"
#include "TypeHash.h"

namespace rhombus
{
	struct TypeInfo final
	{
		std::string_view m_name;
		uint32_t m_size;
		uint32_t m_align;

		template <typename T>
		static constexpr TypeInfo Create();
	};

	class TypeID final
	{
	public:
		constexpr TypeID() = default;
		constexpr TypeID(uint64_t id) : m_id{ id } {};

	public:
		template<typename T>
		static constexpr TypeID Create();

		constexpr void SetTypeID(uint64_t typeID) { m_id = typeID; }
		constexpr uint64_t GetID() const { return m_id; }

		const TypeInfo& GetTypeInfo()	const { return GetTypeInformation(*this); }

		const std::string_view GetTypeName()	const { return GetTypeInfo().m_name; }
		uint32_t GetTypeSize()	const { return GetTypeInfo().m_size; }
		uint32_t GetTypeAlignment()	const { return GetTypeInfo().m_align; }

		template<typename T>
		static TypeInfo& RegisterTypeID()
		{
			auto& typeInfos = GetStatics().TypeInfos;
			const uint64_t typeID = TypeID::Create<T>().GetID();

			RB_CORE_ASSERT(typeInfos.find(typeID) == typeInfos.end(), "Hash collision in TypeInfos!");

			return typeInfos.emplace(typeID, TypeInfo::Create<T>()).first->second;
		}

		static const TypeInfo& GetTypeInformation(TypeID typeID)
		{
			RB_CORE_ASSERT(GetStatics().TypeInfos.find(typeID.GetID()) != GetStatics().TypeInfos.end(), "Type ID is not contained in the type infos array");
			return GetStatics().TypeInfos[typeID.GetID()];
		}

		static const auto& GetAllTypeInformation()
		{
			return GetStatics().TypeInfos;
		}

	private:

		struct StaticData
		{
			std::unordered_map<uint64_t, TypeInfo> TypeInfos{};
		};

		static StaticData& GetStatics()
		{
			// Forces the static data map for always be register first (ahead of any Types)
			static StaticData data{};
			return data;
		}

	private:
		uint64_t m_id{};
	};

	// Register class statically
	template <typename T>
	class RegisterType
	{
	private:
		// This class in a class forces there to be only once type
		class RegisterTypeOnce
		{
		public:
			RegisterTypeOnce()
			{
				TypeID::RegisterTypeID<T>();
			}
		};
		inline static RegisterTypeOnce Register{};
	};

#define _REGISTER_TYPE_INTERNAL(TYPE, VARNAME) RegisterType<TYPE> VARNAME##TYPE {};
#define REGISTER_TYPE(TYPE) _REGISTER_TYPE_INTERNAL(TYPE, RegisterType_)

	// REGISTER_TYPE(int) == 
	// RegisterType<int> RegisterType_int {};
}

template <>
struct std::hash<TypeID>
{
	std::size_t operator()(const TypeID& id) const noexcept
	{
		return static_cast<size_t>(id.GetID());
	}
};

namespace rhombus
{
	template<typename T>
	inline constexpr TypeInfo TypeInfo::Create()
	{
		TypeInfo info{};

		info.m_name = reflection::TypeName<T>();
		info.m_size = sizeof(T);
		info.m_align = alignof(T);

		return info;
	}

	template<typename T>
	inline constexpr TypeID TypeID::Create()
	{
		return TypeID(reflection::TypeID<T>());
	}

	constexpr bool operator==(TypeID lhs, TypeID rhs)
	{
		return lhs.GetID() == rhs.GetID();
	}
}
