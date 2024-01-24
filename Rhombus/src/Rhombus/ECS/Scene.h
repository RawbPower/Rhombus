#pragma once

#include "Rhombus/Core/DeltaTime.h"
#include "Rhombus/Core/UUID.h"
#include "Rhombus/Renderer/EditorCamera.h"

#include <entt.hpp>

class b2World;

namespace rhombus
{
	class Entity;

	class Scene
	{
	public:
		Scene();
		~Scene();

		static Ref<Scene> Copy(Ref<Scene> srcScene);

		Entity CreateEntity(const std::string& name = std::string());
		Entity CreateEntityWithUUID(UUID uuid, const std::string& name = std::string());
		void DestroyEntity(Entity entity);

		void OnRuntimeStart();
		void OnRuntimeStop();

		void OnUpdateRuntime(DeltaTime dt);
		void OnUpdateEditor(DeltaTime dt, EditorCamera& camera);
		void OnViewportResize(uint32_t width, uint32_t height);

		void InitPhyics2D();

		void DuplicateEntity(Entity entity);

		Entity GetEntityByUUID(UUID uuid);

		Entity GetPrimaryCameraEntity();

		template<typename... Components>
		auto GetAllEntitiesWith()
		{
			return m_Registry.view<Components...>();
		}

	private:
		template<typename T>
		void OnComponentAdded(Entity entity, T& component);
	private:
		entt::registry m_Registry;
		uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;

		b2World* m_PhysicsWorld = nullptr;

		std::unordered_map<UUID, entt::entity> m_EntityMap;

		friend class Entity;
		friend class SceneSerializer;
		friend class SceneHierarchyPanel;
	};
}
