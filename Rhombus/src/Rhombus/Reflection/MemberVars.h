#pragma once

#include "rbpch.h"
#include "VariableID.h"
#include <set>

namespace rhombus
{
	class MemberID final
	{
	public:
		struct MemberInfo
		{
			std::string m_name;
			VariableID m_variableID{};
			uint32_t m_offset{};
			uint32_t m_size{};
			uint32_t m_align{};

			constexpr bool operator <(const MemberInfo& rhs) const
			{
				return m_offset < rhs.m_offset;
			}
		};

		constexpr MemberID(TypeID id, uint32_t offset) : m_id{ id }, m_offset{ offset } {};

		constexpr MemberID() = default;
		~MemberID() = default;
		constexpr MemberID(const MemberID&) = default;
		constexpr MemberID& operator=(const MemberID&) = default;
		constexpr MemberID(MemberID&&) noexcept = default;
		constexpr MemberID& operator=(MemberID&&) noexcept = default;

		constexpr TypeID GetTypeID() const { return m_id; }
		constexpr uint32_t GetOffset() const { return m_offset; }

		const MemberInfo& GetMemberInfo() const;
		const std::string& GetName() const { return GetMemberInfo().m_name; }
		VariableID GetVariableID() const { return GetMemberInfo().m_variableID; }
		uint32_t GetSize() const { return GetMemberInfo().m_size; }
		uint32_t GetAlign() const { return GetMemberInfo().m_align; }

		template <typename Class, typename Field>
		static MemberID RegisterField(const std::string& fieldName, uint32_t offset);

		template <typename Field>
		static MemberID RegisterField(TypeID classId, const std::string& fieldName, uint32_t offset);

		static MemberID RegisterField(TypeID classId, VariableID memberId, const std::string& fieldName, uint32_t offset, uint32_t size, uint32_t align);

		static const MemberInfo& GetMemberInfo(TypeID id, const std::string& fieldName);
		static const auto& GetAllMemberInfos() { return GetStatics().MemberInfos; }
		static const std::set<MemberInfo>& GetMemberInfos(TypeID id) { return GetStatics().MemberInfos[id]; }
		static bool Exists(TypeID classId) { return GetStatics().MemberInfos.find(classId) != GetStatics().MemberInfos.end(); }

	private:

		struct StaticData
		{
			std::unordered_map<TypeID, std::set<MemberInfo>> MemberInfos{};
			std::unordered_map<TypeID, std::unordered_map<std::string, uint32_t>> MemberInfoNameMap{};
		};

		static StaticData& GetStatics()
		{
			static StaticData data{};
			return data;
		}

	private:
		TypeID m_id{};
		uint32_t m_offset{};
	};

	inline const MemberID::MemberInfo& MemberID::GetMemberInfo() const
	{
		auto& MemberInfos = GetMemberInfos(m_id);

		MemberInfo InfoToFind{};
		InfoToFind.m_offset = m_offset;

		auto it = MemberInfos.find(InfoToFind);

		// should always contain the value, else something went wrong
		RB_CORE_ASSERT(it != MemberInfos.end(), "[Reflection] Member not found in MemberInfos");

		return *it;
	}

	inline const MemberID::MemberInfo& MemberID::GetMemberInfo(TypeID id, const std::string& fieldName)
	{
		auto& MemberInfoNames = GetStatics().MemberInfoNameMap[id];

		RB_CORE_ASSERT(MemberInfoNames.find(fieldName) != MemberInfoNames.end(), "MemberInfoNames does not contain {0}", fieldName);

		const MemberID MemberId{ id, MemberInfoNames[fieldName] };
		return MemberId.GetMemberInfo();
	}

	struct RegisterMember final
	{
		RegisterMember(TypeID classId, VariableID memberId, const std::string& fieldName, uint32_t offset, uint32_t size, uint32_t align)
		{
			MemberID::RegisterField(classId, memberId, fieldName, offset, size, align);
		}
	};

	inline MemberID MemberID::RegisterField(TypeID classId, VariableID memberId, const std::string& fieldName, uint32_t offset, uint32_t size, uint32_t align)
	{
		MemberInfo info{};
		info.m_name = fieldName;
		info.m_variableID = memberId;
		info.m_offset = offset;
		info.m_size = size;
		info.m_align = align;

		GetStatics().MemberInfos[classId].emplace(info);
		GetStatics().MemberInfoNameMap[classId].emplace(fieldName, offset);

		return { classId, offset };
	}

	template<typename Class, typename Field>
	inline MemberID MemberID::RegisterField(const std::string& fieldName, uint32_t Offset)
	{
		auto registerClass = RegisterType<Class>();
		auto registerField = RegisterType<Field>();

		return RegisterField(
			TypeId::Create<Class>(),
			VariableId::Create<Field>(),
			fieldName,
			Offset,
			sizeof(Field),
			alignof(Field));
	}

	template<typename Field>
	inline MemberID MemberID::RegisterField(TypeID classId, const std::string& fieldName, uint32_t Offset)
	{
		auto registerField = RegisterType<Field>();

		return RegisterField(
			classId,
			VariableId::Create<Field>(),
			fieldName,
			Offset,
			sizeof(Field),
			alignof(Field));
	}
}