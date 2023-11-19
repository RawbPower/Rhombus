#pragma once

#include "Rhombus/Core/DeltaTime.h"

#include <entt.hpp>

namespace rhombus
{
	class Scene
	{
	public:
		Scene();
		~Scene();

		entt::entity CreateEntity();

		// Temp
		entt::registry& Reg() { return m_Registry; }

		void OnUpdate(DeltaTime dt);
	private:
		entt::registry m_Registry;
	};
}
