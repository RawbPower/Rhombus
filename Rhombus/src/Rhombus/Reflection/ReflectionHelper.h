#pragma once

#include "Rhombus/Reflection/MemberVars.h"

namespace rhombus
{
#define _REGISTER_TYPE_INTERNAL(TYPE, VARNAME) RegisterType<TYPE> VARNAME##TYPE {};
#define REGISTER_TYPE(TYPE) _REGISTER_TYPE_INTERNAL(TYPE, RegisterType_)
#define REGISTER_MEMBER(TYPE, FIELD) inline static RegisterMember TYPE##FIELD{TypeID::Create<TYPE>(), VariableID::Create<decltype(TYPE::FIELD)>(), #FIELD, offsetof(TYPE, FIELD), sizeof(decltype(TYPE::FIELD)), alignof(decltype(TYPE::FIELD))};
}

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
	void DrawPropertiesImGUI(T& object, std::function<void(const std::string&, const std::string&, const void*)> uiFunction)
	{
		for (auto& info : MemberID::GetMemberInfos(TypeID::Create<T>()))
		{
			const std::string& unitName = GetVariableName(info.m_variableID);
			const void* testPtr = &object;
			const void* unitPtr = static_cast<const uint8_t*>(testPtr) + info.m_offset;
			uiFunction(info.m_name, unitName, unitPtr);
		}
	}
}
