#include "rbpch.h"
#include "Scene.h"
#include "Component.h"
#include "Entity.h"
#include "ScriptableEntity.h"
#include "Rhombus/Renderer/Renderer2D.h"
#include "Rhombus/Scripting/ScriptEngine.h"

#include <glm/glm.hpp>

// Box2d
#include <box2d/b2_world.h>
#include <box2d/b2_body.h>
#include <box2d/b2_fixture.h>
#include <box2d/b2_polygon_shape.h>
#include <box2d/b2_circle_shape.h>

namespace rhombus
{
	static b2BodyType Rigidbody2DTypetoBox2DType(Rigidbody2DComponent::BodyType bodyType)
	{
		switch (bodyType)
		{
		case Rigidbody2DComponent::BodyType::Static:		return b2_staticBody;
		case Rigidbody2DComponent::BodyType::Dynamic:		return b2_dynamicBody;
		case Rigidbody2DComponent::BodyType::Kinematic:		return b2_kinematicBody;
		}

		RB_CORE_ASSERT(false, "Unkown body type");
		return b2_staticBody;
	}

	Scene::Scene()
	{

	}

	Scene::~Scene()
	{
		delete m_PhysicsWorld;		// just incase
	}

	template<typename... Component>
	static void CopyComponent(entt::registry& dest, const entt::registry& src, const	std::unordered_map<UUID, entt::entity>& enttMap)
	{
		([&]()
		{
			auto view = src.view<Component>();
			for (auto e : view)
			{
				UUID uuid = src.get<IDComponent>(e).m_id;
				entt::entity destEntityID = enttMap.at(uuid);

				auto& component = src.get<Component>(e);
				dest.emplace_or_replace<Component>(destEntityID, component);
			}
		}(), ...);
	}

	template<typename... Component>
	static void CopyComponent(ComponentGroup<Component...>, entt::registry& dst, entt::registry& src, const std::unordered_map<UUID, entt::entity>& enttMap)
	{
		CopyComponent<Component...>(dst, src, enttMap);
	}

	template<typename... Component>
	static void CopyComponentIfExists(Entity dest, Entity src)
	{
		([&]()
		{
			if (src.HasComponent<Component>())
				dest.AddOrReplaceComponent<Component>(src.GetComponent<Component>());
		}(), ...);
	}

	template<typename... Component>
	static void CopyComponentIfExists(ComponentGroup<Component...>, Entity dst, Entity src)
	{
		CopyComponentIfExists<Component...>(dst, src);
	}

	Ref<Scene> Scene::Copy(Ref<Scene> srcScene)
	{
		Ref<Scene> destScene = CreateRef<Scene>();

		destScene->m_ViewportHeight = srcScene->m_ViewportHeight;
		destScene->m_ViewportWidth = srcScene->m_ViewportWidth;

		auto& srcSceneRegistry = srcScene->m_Registry;
		auto& destSceneRegistry = destScene->m_Registry;
		std::unordered_map<UUID, entt::entity> enttMap;

		// Create new entities for scene
		auto idView = srcSceneRegistry.view<IDComponent>();		// Every entity
		for (auto e : idView)
		{
			UUID uuid = srcSceneRegistry.get<IDComponent>(e).m_id;
			const auto& name = srcSceneRegistry.get<TagComponent>(e).m_tag;
			Entity newEntity = destScene->CreateEntityWithUUID(uuid, name);
			enttMap[uuid] = (entt::entity)newEntity;
		}

		// Copy components (except IDComponent and TagComponent)
		CopyComponent(AllComponents{}, destSceneRegistry, srcSceneRegistry, enttMap);

		return destScene;
	}

	Entity Scene::CreateEntity(const std::string& name)
	{
		return CreateEntityWithUUID(UUID(), name);
	}

	Entity Scene::CreateEntityWithUUID(UUID uuid, const std::string& name)
	{
		Entity entity(m_Registry.create(), this);
		entity.AddComponent<IDComponent>(uuid);
		entity.AddComponent<TransformComponent>();
		auto& tag = entity.AddComponent<TagComponent>();
		tag.m_tag = name.empty() ? "Entity" : name;

		m_EntityMap[uuid] = entity;

		return entity;
	}

	void Scene::DestroyEntity(Entity entity)
	{
		m_Registry.destroy(entity);
		m_EntityMap.erase(entity.GetUUID());
	}

	void Scene::OnRuntimeStart()
	{
		InitPhyics2D();

		// Scripting
		ScriptEngine::OnRuntimeStart(this);

		// Instantiate all the script components
		auto view = m_Registry.view<ScriptComponent>();
		for (auto e : view)
		{
			Entity entity = { e, this };
			ScriptEngine::OnInitEntity(entity);
		}
	}

	void Scene::OnRuntimeStop()
	{
		delete m_PhysicsWorld;
		m_PhysicsWorld = nullptr;

		ScriptEngine::OnRuntimeStop();
	}

	void Scene::OnUpdateRuntime(DeltaTime dt)
	{
		// Update Scripts
		{
			// Lua
			auto view = m_Registry.view<ScriptComponent>();
			for (auto e : view)
			{
				Entity entity = { e, this };
				ScriptEngine::OnUpdateEntity(entity, dt);
			}

			// Native
			m_Registry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc)
			{
					// TODO: Move on Scene::OnScenePlay
				if (!nsc.m_instance)
				{
					nsc.m_instance = nsc.InstantiateScript();
					nsc.m_instance->m_entity = Entity(entity, this);
					//nsc.m_instance->m_entity = Entity{entity, this}; ?????
					nsc.m_instance->OnReady();
				}

				nsc.m_instance->OnUpdate(dt);
			});
		}

		// Physics
		{
			const int32_t velocityIterations = 6;
			const int32_t positionIterations = 2;
			m_PhysicsWorld->Step(dt, velocityIterations, positionIterations);

			// Retrieve transform post physics step
			auto view = m_Registry.view<Rigidbody2DComponent>();
			for (auto e : view)
			{
				Entity entity = { e, this };
				auto& transform = entity.GetComponent<TransformComponent>();
				auto& rb = entity.GetComponent<Rigidbody2DComponent>();

				b2Body* body = (b2Body*)rb.m_runtimeBody;
				const auto& position = body->GetPosition();
				transform.m_position.x = position.x;
				transform.m_position.y = position.y;
				transform.m_rotation.z = body->GetAngle();
			}
		}

		Camera* mainCamera = nullptr;
		glm::mat4 cameraTransform;
		{
			auto view = m_Registry.view<TransformComponent, CameraComponent>();
			for (auto entity : view)
			{
				auto [transformComponent, cameraComponent] = view.get<TransformComponent, CameraComponent>(entity);

				if (cameraComponent.GetIsPrimaryCamera())
				{
					mainCamera = &cameraComponent.GetCamera();
					cameraTransform = transformComponent.GetTransform();
					break;
				}
			}
		}

		// Render Sprites
		if (mainCamera)
		{
			Renderer2D::BeginScene(*mainCamera, cameraTransform);

			// TODO: Account for all types of rendering when ordering the draw calls

			// Draw sprites
			{
				// To make blending work for multiple objects we have to draw the
				// most distant object first and the closest object last
				m_Registry.sort<SpriteRendererComponent>([&](const entt::entity lhs, const entt::entity rhs) {
					return m_Registry.get<TransformComponent>(lhs).m_position.z < m_Registry.get<TransformComponent>(rhs).m_position.z;
					});

				auto view = m_Registry.view<SpriteRendererComponent, TransformComponent>();

				for (auto entity : view)
				{
					auto [spriteRendererComponent, transformComponent] = view.get<SpriteRendererComponent, TransformComponent>(entity);

					Renderer2D::DrawSprite(transformComponent.GetTransform(), spriteRendererComponent, (int)entity);
				}
			}

			// Draw circles
			{
				// To make blending work for multiple objects we have to draw the
				// most distant object first and the closest object last
				m_Registry.sort<CircleRendererComponent>([&](const entt::entity lhs, const entt::entity rhs) {
					return m_Registry.get<TransformComponent>(lhs).m_position.z < m_Registry.get<TransformComponent>(rhs).m_position.z;
				});

				auto view = m_Registry.view<CircleRendererComponent, TransformComponent>();

				for (auto entity : view)
				{
					auto [circleRendererComponent, transformComponent] = view.get<CircleRendererComponent, TransformComponent>(entity);

					Renderer2D::DrawCircle(transformComponent.GetTransform(), circleRendererComponent.m_color,
						circleRendererComponent.m_thickness, circleRendererComponent.m_fade, (int)entity);
				}
			}

			Renderer2D::EndScene();
		}
	}

	void Scene::OnUpdateEditor(DeltaTime dt, EditorCamera& camera)
	{
		Renderer2D::BeginScene(camera);

		// TODO: Account for all types of rendering when ordering the draw calls

		// Draw Sprites
		{
			// To make blending work for multiple objects we have to draw the
			// most distant object first and the closest object last
			m_Registry.sort<SpriteRendererComponent>([&](const entt::entity lhs, const entt::entity rhs) {
				return m_Registry.get<TransformComponent>(lhs).m_position.z < m_Registry.get<TransformComponent>(rhs).m_position.z;
				});

			auto view = m_Registry.view<SpriteRendererComponent, TransformComponent>();

			for (auto entity : view)
			{
				auto [spriteRendererComponent, transformComponent] = view.get<SpriteRendererComponent, TransformComponent>(entity);

				Renderer2D::DrawSprite(transformComponent.GetTransform(), spriteRendererComponent, (int)entity);
			}
		}

		// Draw circles
		{
			// To make blending work for multiple objects we have to draw the
			// most distant object first and the closest object last
			m_Registry.sort<CircleRendererComponent>([&](const entt::entity lhs, const entt::entity rhs) {
				return m_Registry.get<TransformComponent>(lhs).m_position.z < m_Registry.get<TransformComponent>(rhs).m_position.z;
				});

			auto view = m_Registry.view<CircleRendererComponent, TransformComponent>();

			for (auto entity : view)
			{
				auto [circleRendererComponent, transformComponent] = view.get<CircleRendererComponent, TransformComponent>(entity);

				Renderer2D::DrawCircle(transformComponent.GetTransform(), circleRendererComponent.m_color,
					circleRendererComponent.m_thickness, circleRendererComponent.m_fade, (int)entity);
			}
		}

		Renderer2D::EndScene();
	}

	void Scene::OnViewportResize(uint32_t width, uint32_t height)
	{
		m_ViewportWidth = width;
		m_ViewportHeight = height;

		// Resize our non-fixed aspect ratio cameras
		auto view = m_Registry.view<CameraComponent>();
		for (auto entity : view)
		{
			auto& cameraComponent = view.get<CameraComponent>(entity);
			if (!cameraComponent.GetHasFixedAspectRatio())
			{
				cameraComponent.GetCamera().SetViewportResize(width, height);
			}
		}
	}

	void Scene::InitPhyics2D()
	{
		m_PhysicsWorld = new b2World({ 0.0f, -9.8f });

		auto view = m_Registry.view<Rigidbody2DComponent>();
		for (auto e : view)
		{
			Entity entity = { e, this };
			auto& transform = entity.GetComponent<TransformComponent>();
			auto& rb = entity.GetComponent<Rigidbody2DComponent>();

			b2BodyDef bodyDef;
			bodyDef.type = Rigidbody2DTypetoBox2DType(rb.m_type);
			bodyDef.position.Set(transform.m_position.x, transform.m_position.y);
			bodyDef.angle = transform.m_rotation.z;

			b2Body* body = m_PhysicsWorld->CreateBody(&bodyDef);
			body->SetFixedRotation(rb.m_fixedRotation);
			rb.m_runtimeBody = body;

			if (entity.HasComponent<BoxCollider2DComponent>())
			{
				auto& collider = entity.GetComponent<BoxCollider2DComponent>();

				b2PolygonShape boxShape;
				boxShape.SetAsBox(collider.m_size.x * transform.m_scale.x, collider.m_size.y * transform.m_scale.y);

				b2FixtureDef fixtureDef;
				fixtureDef.shape = &boxShape;
				fixtureDef.density = collider.m_density;
				fixtureDef.friction = collider.m_friction;
				fixtureDef.restitution = collider.m_restitution;
				fixtureDef.restitutionThreshold = collider.m_restitutionThreshold;
				body->CreateFixture(&fixtureDef);
			}

			if (entity.HasComponent<CircleCollider2DComponent>())
			{
				auto& collider = entity.GetComponent<CircleCollider2DComponent>();

				b2CircleShape circleShape;
				circleShape.m_p.Set(collider.m_offset.x, collider.m_offset.y);
				circleShape.m_radius = transform.m_scale.x * collider.m_radius;

				b2FixtureDef fixtureDef;
				fixtureDef.shape = &circleShape;
				fixtureDef.density = collider.m_density;
				fixtureDef.friction = collider.m_friction;
				fixtureDef.restitution = collider.m_restitution;
				fixtureDef.restitutionThreshold = collider.m_restitutionThreshold;
				body->CreateFixture(&fixtureDef);
			}
		}
	}

	void Scene::DuplicateEntity(Entity entity)
	{
		std::string name = entity.GetName();
		Entity newEntity = CreateEntity(name);

		CopyComponentIfExists(AllComponents{}, newEntity, entity);
	}

	Entity Scene::GetEntityByUUID(UUID uuid)
	{
		// TODO(Yan): Maybe should be assert
		if (m_EntityMap.find(uuid) != m_EntityMap.end())
			return { m_EntityMap.at(uuid), this };

		return {};
	}

	Entity Scene::GetPrimaryCameraEntity()
	{
		auto view = m_Registry.view<CameraComponent>();
		for (auto entity : view)
		{
			auto& camera = view.get<CameraComponent>(entity);
			if (camera.GetIsPrimaryCamera())
			{
				return Entity{ entity, this };
			}
		}

		return {};
	}

	// Not sure about this template specialization stuff :/
	template<typename T>
	void Scene::OnComponentAdded(Entity entity, T& component)
	{
		static_assert(sizeof(T) == 0);
	}

	template<>
	void Scene::OnComponentAdded<IDComponent>(Entity entity, IDComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<TransformComponent>(Entity entity, TransformComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<CameraComponent>(Entity entity, CameraComponent& component)
	{
		if (m_ViewportWidth > 0 && m_ViewportHeight > 0)
		{
			component.GetCamera().SetViewportResize(m_ViewportWidth, m_ViewportHeight);
		}
	}

	template<>
	void Scene::OnComponentAdded<TagComponent>(Entity entity, TagComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<ScriptComponent>(Entity entity, ScriptComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<SpriteRendererComponent>(Entity entity, SpriteRendererComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<CircleRendererComponent>(Entity entity, CircleRendererComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<NativeScriptComponent>(Entity entity, NativeScriptComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<Rigidbody2DComponent>(Entity entity, Rigidbody2DComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<BoxCollider2DComponent>(Entity entity, BoxCollider2DComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<CircleCollider2DComponent>(Entity entity, CircleCollider2DComponent& component)
	{
	}
}