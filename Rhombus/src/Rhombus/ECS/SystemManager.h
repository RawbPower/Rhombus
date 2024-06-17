#pragma once

#include "ECSTypes.h"
#include "System.h"
#include "Rhombus/Core/Log.h"

#include <unordered_map>

namespace rhombus
{
	class Scene;

	class SystemManager
	{
	public:
		void OnEntityDestroyed(EntityID entity);

		void OnEntitySignatureChanged(EntityID entity, Signature entitySignature);

		template<typename T>
		Ref<T> RegsiterSystem(Scene* scene)
		{
			const char* typeName = typeid(T).name();

			RB_CORE_ASSERT(m_systems.find(typeName) == m_systems.end(), "Register system more than once.");

			// Create a pointer to the system and return it so it can be used externally
			auto system = std::make_shared<T>(scene);
			m_systems.insert({ typeName, system });
			return system;
		}

		template<typename T>
		void SetSignature(Signature signature)
		{
			const char* typeName = typeid(T).name();

			RB_CORE_ASSERT(m_systems.find(typeName) != m_systems.end(), "System used before registered.");

			// Set the signature for this system
			m_signatures.insert({ typeName, signature });
		}

	private:
		// Map from system type string pointer to a signture
		std::unordered_map<const char*, Signature> m_signatures{};

		// Map from system type string pointer to a system pointer
		std::unordered_map<const char*, Ref<System>> m_systems{};
	};
}
