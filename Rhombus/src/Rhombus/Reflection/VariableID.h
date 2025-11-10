#pragma once

#include "TypeInfo.h"

#include <type_traits>

namespace rhombus
{
	class VariableID final
	{
	private:
		static constexpr uint32_t ConstFlag = 1 << 0;
		static constexpr uint32_t ReferenceFlag = 1 << 1;
		static constexpr uint32_t VolatileFlag = 1 << 2;
		static constexpr uint32_t RValReferenceFlag = 1 << 3;

	public:

		constexpr explicit VariableID(TypeID id) : m_type{ id } {};
		constexpr VariableID() = default;

		template <typename T>
		static constexpr VariableID Create();

		constexpr TypeID GetTypeID() const { return m_type; }
		constexpr void SetTypeID(TypeID id) { m_type = id; }

		constexpr void SetConstFlag() { m_traitFlags |= ConstFlag; }
		constexpr void SetReferenceFlag() { m_traitFlags |= ReferenceFlag; }
		constexpr void SetVolatileFlag() { m_traitFlags |= VolatileFlag; }
		constexpr void SetRValReferenceFlag() { m_traitFlags |= RValReferenceFlag; }

		constexpr void RemoveConstFlag() { m_traitFlags &= ~ConstFlag; }
		constexpr void RemoveReferenceFlag() { m_traitFlags &= ~ReferenceFlag; }
		constexpr void RemoveVolatileFlag() { m_traitFlags &= ~VolatileFlag; }
		constexpr void RemoveRValReferenceFlag() { m_traitFlags &= ~RValReferenceFlag; }

		constexpr void SetPointerCount(uint32_t count) { m_pointerCount = count; }
		constexpr uint32_t GetPointerCount() const { return m_pointerCount; }

		constexpr void SetArraySize(uint32_t size) { m_arraySize = size; }
		constexpr uint32_t GetArraySize() const { return m_arraySize; }

		constexpr bool IsConst() const { return m_traitFlags & ConstFlag; }
		constexpr bool IsReference() const { return m_traitFlags & ReferenceFlag; }
		constexpr bool IsVolatile() const { return m_traitFlags & VolatileFlag; }
		constexpr bool IsRValReference() const { return m_traitFlags & RValReferenceFlag; }
		constexpr bool IsPointer() const { return m_pointerCount > 0; }
		constexpr bool IsArray() const { return m_arraySize > 1; }
		constexpr bool IsRefOrPointer() const { return IsPointer() || IsReference() || IsRValReference(); }

		constexpr uint32_t GetSize() const { return IsRefOrPointer() ? sizeof(void*) : (GetTypeID().GetTypeSize() * GetArraySize()); }
		constexpr uint32_t GetAlign() const { return IsRefOrPointer() ? alignof(void*) : GetTypeID().GetTypeAlignment(); }

		constexpr uint64_t GetHash() const { return m_type.GetID() ^ m_arraySize ^ (static_cast<uint64_t>(m_pointerCount) << 32) ^ (static_cast<uint64_t>(m_traitFlags) << 40); }

		friend bool operator==(const VariableID& lhs, const VariableID& rhs);

	private:
		TypeID m_type{};			// The underlying type id
		uint32_t m_arraySize{};		// If the variable is a fixed size array, the size will be contained in this. Otherwise it will be 1
		uint32_t m_pointerCount{};	// Thje amount of pointers that are attached to the Type
		uint8_t m_traitFlags{};		// Other flags (const, reference)
	};

	template <typename T>
	constexpr uint32_t CountPointers(uint32_t counter = 0)
	{
		if constexpr (std::is_pointer_v<T>)
		{
			return CountPointers<std::remove_pointer_t<T>>(++counter);
		}
		else
		{
			return counter;
		}
	}

	//https://stackoverflow.com/questions/9851594/standard-c11-way-to-remove-all-pointers-of-a-type

	// your custom 'identity' meta function
	template <typename T>
	struct identity
	{
		using type = T;
	};

	template<typename T>
	struct remove_all_pointers : std::conditional_t<
		std::is_pointer_v<T>,
		remove_all_pointers<
		std::remove_pointer_t<T>
		>,
		identity<T>
	>
	{
	};

	template<typename T>
	using remove_all_pointers_t = typename remove_all_pointers<T>::type;

	template<typename T>
	inline constexpr VariableID VariableID::Create()
	{
		using Type_RemovedExtents = std::remove_all_extents_t<T>;
		using Type_RemovedRefs = std::remove_reference_t<Type_RemovedExtents>;
		using Type_RemovedPtrs = remove_all_pointers_t<Type_RemovedRefs>;

		using StrippedType = std::_Remove_cvref_t<Type_RemovedPtrs>;
		RegisterType<StrippedType> TypeRegister{};

		constexpr bool IsRef{ std::is_reference_v<T> };
		constexpr bool IsRValRef{ std::is_rvalue_reference_v<T> };
		constexpr bool IsConst{ std::is_const_v<Type_RemovedPtrs> };
		constexpr bool IsVolatile{ std::is_volatile_v<Type_RemovedPtrs> };

		constexpr uint32_t PointerCount{ CountPointers<Type_RemovedRefs>() };

		auto variable = VariableID(TypeID::Create<StrippedType>());

		if constexpr (IsConst) variable.SetConstFlag();
		if constexpr (IsVolatile) variable.SetVolatileFlag();
		if constexpr (IsRef) variable.SetReferenceFlag();
		if constexpr (IsRValRef) variable.SetRValReferenceFlag();

		variable.SetPointerCount(PointerCount);

		if constexpr (!std::is_same_v<void, Type_RemovedExtents>)
		{
			constexpr uint32_t ArraySize{ sizeof(T) / sizeof(Type_RemovedExtents) };
			variable.SetArraySize(ArraySize);
		}
		else
		{
			variable.SetArraySize(1);
		}

		return variable;
	}
}

template <>
struct std::hash<VariableID>
{
	std::size_t operator()(const VariableID& id) const noexcept
	{
		return static_cast<size_t>(id.GetHash());
	}
};

namespace rhombus
{
	bool operator==(const VariableID& lhs, const VariableID& rhs)
	{
		return lhs.m_type == rhs.m_type &&
			lhs.m_arraySize == rhs.m_arraySize &&
			lhs.m_pointerCount == rhs.m_pointerCount &&
			lhs.m_traitFlags == rhs.m_traitFlags;
	}

	inline const std::string& GetVariableName(const VariableID& variableID)
	{
		static std::unordered_map<VariableID, std::string> VariableNames{};

		auto it = VariableNames.find(variableID);
		if (it != VariableNames.end())
		{
			return it->second;
		}

		{
			std::string Name = std::string(variableID.GetTypeID().GetTypeName());

			if (variableID.IsVolatile()) Name = "volatile " + Name;
			if (variableID.IsConst()) Name = "const " + Name;

			const uint32_t pointerCount = variableID.GetPointerCount();
			for (uint32_t i = 0; i < pointerCount; i++)
			{
				Name += '*';
			}

			if (variableID.GetArraySize() > 1)
			{
				Name += '[';
				Name += std::to_string(variableID.GetArraySize());
				Name += ']';
			}

			if (variableID.IsRValReference()) Name += "&&";
			else if (variableID.IsReference()) Name += '&';

			return VariableNames.emplace(variableID, std::move(Name)).first->second;
		}
	}
}
