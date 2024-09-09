#pragma once

#include "ECSTypes.h"
#include "Registry.h"

#include "Rhombus/Core/DeltaTime.h"
#include "Rhombus/Core/UUID.h"
#include "Rhombus/Renderer/EditorCamera.h"
#include "Rhombus/ECS/Systems/TweeningSystem.h"
class b2World;

namespace rhombus
{
	class Entity;
	class Tween;

	template <typename... Component>
	struct ComponentGroup
	{
	};

	class Scene
	{
	public:
		Scene();
		~Scene();

		virtual void InitScene();

		static void Copy(Ref<Scene> destScene, Ref<Scene> srcScene);

		virtual void CopyAllComponents(Ref<Scene> destScene, const std::unordered_map<UUID, EntityID>& entityMap);
		virtual void CopyEntityComponents(Entity dest, Entity src);

		Entity CreateEntity(const std::string& name = std::string());
		Entity CreateEntityWithUUID(UUID uuid, const std::string& name = std::string());
		void DestroyEntity(Entity entity);

		virtual void OnRuntimeStart();
		virtual void OnRuntimeStop();

		virtual void OnUpdateRuntime(DeltaTime dt);
		virtual void OnDraw();
		void OnUpdateEditor(DeltaTime dt, EditorCamera& camera);
		virtual void OnMouseMoved(int x, int y);
		virtual void OnMouseButtonPressed(int button);
		virtual void OnMouseButtonReleased(int button);
		void OnViewportResize(uint32_t width, uint32_t height);

		virtual void SerializeEntity(void* yamlEmitter, Entity entity);
		virtual void DeserializeEntity(void* yamlEntity, Entity entity);

		void InitPhyics2D();

		Ref<Tween> CreateTween(Entity entity, Vec3* param, Vec3 begin, Vec3 finish, float duration);

		void DuplicateEntity(Entity entity);

		// Adds appropriate number suffix to this entity name
		std::string GetNameForDuplicate(Entity entity);
		bool DoesNameExistInScene(std::string name);

		Entity GetEntityByUUID(UUID uuid);

		Entity GetPrimaryCameraEntity();

		std::vector<EntityID> GetAllEntities();

		template<typename T>
		std::vector<EntityID> GetAllEntitiesWith()
		{
			return m_Registry.GetEntityList<T>();
		}

		Registry& GetRegistry()
		{
			return m_Registry;
		}

	protected:
		// Component Registration
		template<typename... Component>
		inline void RegisterComponents()
		{
			([&]()
				{
					m_Registry.RegisterComponent<Component>();
				}(), ...);
		}

		template<typename... Component>
		inline void RegisterComponents(ComponentGroup<Component...>)
		{
			RegisterComponents<Component...>();
		}

		// Component Copying
		template<typename... Component>
		inline static void CopyComponent(Ref<Scene> destScene, const Registry& src, const	std::unordered_map<UUID, EntityID>& entityMap)
		{
			([&]()
				{
					std::vector<EntityID> view = src.GetEntityList<Component>();
					for (auto e : view)
					{
						UUID uuid = src.GetComponent<IDComponent>(e).m_id;
						EntityID destEntityID = entityMap.at(uuid);

						auto& component = src.GetComponent<Component>(e);
						auto& destComponent = destScene->m_Registry.AddOrReplaceComponent<Component>(destEntityID, component);
						destComponent.SetOwnerEntity(destEntityID, destScene.get());
					}
				}(), ...);
		}

		template<typename... Component>
		inline static void CopyComponent(ComponentGroup<Component...>, Ref<Scene> dst, Registry& src, const std::unordered_map<UUID, EntityID>& entityMap)
		{
			CopyComponent<Component...>(dst, src, entityMap);
		}

		template<typename... Component>
		inline static void CopyComponentIfExists(Entity dest, Entity src)
		{
			([&]()
				{
					if (src.HasComponent<Component>())
						dest.AddOrReplaceComponent<Component>(src.GetComponent<Component>());
				}(), ...);
		}

		template<typename... Component>
		inline static void CopyComponentIfExists(ComponentGroup<Component...>, Entity dst, Entity src)
		{
			CopyComponentIfExists<Component...>(dst, src);
		}

	protected:
		Registry m_Registry;
		uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;

		b2World* m_PhysicsWorld = nullptr;
		Ref<TweeningSystem> tweeningSystem;

		std::unordered_map<UUID, EntityID> m_EntityMap;

		friend class Entity;
		friend class SceneSerializer;
		friend class SceneHierarchyPanel;
	};
}
