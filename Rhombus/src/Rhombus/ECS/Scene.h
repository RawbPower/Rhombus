#pragma once

#include "ECSTypes.h"
#include "Registry.h"

#include "Rhombus/Core/DeltaTime.h"
#include "Rhombus/Core/UUID.h"
#include "Rhombus/Renderer/EditorCamera.h"

class b2World;

namespace rhombus
{
	class Entity;

	class Scene
	{
	public:
		Scene();
		~Scene();

		virtual void InitScene();

		static void Copy(Ref<Scene> destScene, Ref<Scene> srcScene);

		Entity CreateEntity(const std::string& name = std::string());
		Entity CreateEntityWithUUID(UUID uuid, const std::string& name = std::string());
		void DestroyEntity(Entity entity);

		virtual void OnRuntimeStart();
		virtual void OnRuntimeStop();

		virtual void OnUpdateRuntime(DeltaTime dt);
		void OnUpdateEditor(DeltaTime dt, EditorCamera& camera);
		virtual void OnMouseMoved(int x, int y);
		virtual void OnMouseButtonPressed(int button);
		virtual void OnMouseButtonReleased(int button);
		void OnViewportResize(uint32_t width, uint32_t height);

		virtual void SerializeEntity(void* yamlEmitter, Entity entity);
		virtual void DeserializeEntity(void* yamlEntity, Entity entity);

		void InitPhyics2D();

		void DuplicateEntity(Entity entity);

		Entity GetEntityByUUID(UUID uuid);

		Entity GetPrimaryCameraEntity();

		std::vector<EntityID> GetAllEntities();

		template<typename T>
		std::vector<EntityID> GetAllEntitiesWith()
		{
			return m_Registry.GetEntityList<T>();
		}

	protected:
		template<typename T>
		void OnComponentAdded(Entity entity, T& component);
	protected:
		Registry m_Registry;
		uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;

		b2World* m_PhysicsWorld = nullptr;

		std::unordered_map<UUID, EntityID> m_EntityMap;

		friend class Entity;
		friend class SceneSerializer;
		friend class SceneHierarchyPanel;
	};
}
