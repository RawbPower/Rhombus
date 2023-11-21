#pragma once

#include "Rhombus/Core/DeltaTime.h"

#include <entt.hpp>

namespace rhombus
{
	class Entity;

	class Scene
	{
	public:
		Scene();
		~Scene();

		Entity CreateEntity(const std::string& name = std::string());

		void OnUpdate(DeltaTime dt);
	private:
		entt::registry m_Registry;
		friend class Entity;
	};
}
