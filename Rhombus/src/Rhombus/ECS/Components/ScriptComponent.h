#pragma once

#include "Component.h"

namespace rhombus
{
	class ScriptComponent : public ComponentBase
	{
	public:
		ScriptComponent() = default;
		ScriptComponent(const ScriptComponent&) = default;

		std::string m_scriptName;
	};

	// Forward declaration
	class ScriptableEntity;

	class NativeScriptComponent : public ComponentBase
	{
	public:
		ScriptableEntity* (*InstantiateScript)();
		void (*DestroyScript)(NativeScriptComponent*);

		template<typename T>
		void Bind()
		{
			InstantiateScript = []() { return static_cast<ScriptableEntity*>(new T()); };
			DestroyScript = [](NativeScriptComponent* nsc) { delete nsc->m_instance; nsc->m_instance = nullptr; };
		}

	private:
		ScriptableEntity* m_instance = nullptr;
		friend class Scene;
	};
}
