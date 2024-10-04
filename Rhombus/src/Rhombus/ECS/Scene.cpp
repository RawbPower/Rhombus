#include "rbpch.h"
#include "Scene.h"
#include "Components/Component.h"
#include "Components/Rigidbody2DComponent.h"
#include "Entity.h"
#include "ScriptableEntity.h"
#include "Rhombus/Renderer/Renderer2D.h"
#include "Rhombus/Scripting/ScriptEngine.h"
#include "Rhombus/Core/Application.h"

// To Remove
#include "Rhombus/ECS/Components/Area2DComponent.h"
#include "Rhombus/ECS/Components/CameraComponent.h"
#include "Rhombus/ECS/Components/CircleRendererComponent.h"
#include "Rhombus/ECS/Components/Collider2DComponent.h"
//#include "Rhombus/ECS/Components/Rigidbody2DComponent.h"
#include "Rhombus/ECS/Components/ScriptComponent.h"
#include "Rhombus/ECS/Components/SpriteRendererComponent.h"
#include "Rhombus/ECS/Components/TransformComponent.h"
#include "Rhombus/ECS/Components/TweenComponent.h"

// Box2d
#include <box2d/b2_world.h>
#include <box2d/b2_body.h>
#include <box2d/b2_fixture.h>
#include <box2d/b2_polygon_shape.h>
#include <box2d/b2_circle_shape.h>

namespace rhombus
{
	// Update this list when any new components are added
	// -----------------------------------------------------
	using RhombusComponents =
		ComponentGroup<TransformComponent, SpriteRendererComponent,
		CircleRendererComponent, CameraComponent, ScriptComponent, NativeScriptComponent,
		Rigidbody2DComponent, BoxCollider2DComponent, CircleCollider2DComponent, BoxArea2DComponent, TweenComponent>;
	// -----------------------------------------------------

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
		InitScene();
	}

	Scene::~Scene()
	{
		delete m_PhysicsWorld;		// just incase
	}

	void Scene::InitScene()
	{
		m_Registry.Init();
		
		// ID and tag components are unique so shouldn't be grouped with 
		// the rest of the components in general. It is created with the 
		// entity and doesn't need to be copied
		m_Registry.RegisterComponent<IDComponent>();
		m_Registry.RegisterComponent<TagComponent>();

		RegisterComponents(RhombusComponents{});

		tweeningSystem = m_Registry.RegisterSystem<TweeningSystem>(this);
		{
			Signature signature;
			signature.set(m_Registry.GetComponentType<TweenComponent>());
			m_Registry.SetSystemSignature<TweeningSystem>(signature);
		}
	}

	void Scene::Copy(Ref<Scene> destScene, Ref<Scene> srcScene)
	{
		destScene->m_ViewportHeight = srcScene->m_ViewportHeight;
		destScene->m_ViewportWidth = srcScene->m_ViewportWidth;

		auto& srcSceneRegistry = srcScene->m_Registry;
		auto& destSceneRegistry = destScene->m_Registry;
		std::unordered_map<UUID, EntityID> entityMap;

		// Create new entities for scene
		std::vector<EntityID> idView = srcSceneRegistry.GetEntityList<IDComponent>();		// Every entity
		for (EntityID e : idView)
		{
			if (srcScene->IsEntityDisabled(e))
			{
				continue;
			}

			UUID uuid = srcSceneRegistry.GetComponent<IDComponent>(e).m_id;
			const auto& name = srcSceneRegistry.GetComponent<TagComponent>(e).m_tag;
			Entity newEntity = destScene->CreateEntityWithUUID(uuid, name);
			entityMap[uuid] = (EntityID)newEntity;
		}

		// Copy components (TODO: Skip ID Component?)
		//destScene->m_Registry.CopyComponents(srcScene->m_Registry);
		srcScene->CopyAllComponents(destScene, entityMap);
	}

	void Scene::CopyAllComponents(Ref<Scene> destScene, const std::unordered_map<UUID, EntityID>& entityMap)
	{
		CopyComponent(RhombusComponents{}, destScene, m_Registry, entityMap);
	}

	Entity Scene::CreateEntity(const std::string& name)
	{
		return CreateEntityWithUUID(UUID(), name);
	}

	Entity Scene::CreateEntityWithUUID(UUID uuid, const std::string& name)
	{
		Entity entity(m_Registry.CreateEntity(), this);
		IDComponent& idComponent = entity.AddComponent(IDComponent(uuid));
		//IDComponent& idComponent = entity.AddComponent<IDComponent>();
		//idComponent.m_id = uuid;
		entity.AddComponent<TransformComponent>();
		auto& tag = entity.AddComponent<TagComponent>();
		tag.m_tag = name.empty() ? "Entity" : name;

		m_EntityMap[uuid] = entity;

		return entity;
	}

	void Scene::DestroyEntity(Entity entity)
	{
		UUID entityUUID = entity.GetUUID();
		m_Registry.DestroyEntity(entity);
		m_EntityMap.erase(entityUUID);
	}

	void Scene::OnRuntimeStart()
	{
		InitPhyics2D();

		// Scripting
		ScriptEngine::OnRuntimeStart(this);

		// Instantiate all the script components
		std::vector<EntityID> view = m_Registry.GetEntityList<ScriptComponent>();
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
		if (!Application::Get().GetIsDebugPaused())
		{
			// Update Scripts
			{
				// Lua
				std::vector<EntityID> view = m_Registry.GetEntityList<ScriptComponent>();
				for (auto e : view)
				{
					Entity entity = { e, this };
					ScriptEngine::OnUpdateEntity(entity, dt);
				}

				// Native
				std::vector<EntityID> nativeView = m_Registry.GetEntityList<NativeScriptComponent>();
				for (auto e : nativeView)
				{
					NativeScriptComponent& nsc = m_Registry.GetComponent<NativeScriptComponent>(e);
					// TODO: Move on Scene::OnScenePlay
					if (!nsc.m_instance)
					{
						nsc.m_instance = nsc.InstantiateScript();
						nsc.m_instance->m_entity = Entity(e, this);
						//nsc.m_instance->m_entity = Entity{entity, this}; ?????
						nsc.m_instance->OnReady();
					}

					nsc.m_instance->OnUpdate(dt);
				}
			}

			// Physics
			{
				const int32_t velocityIterations = 6;
				const int32_t positionIterations = 2;
				m_PhysicsWorld->Step(dt, velocityIterations, positionIterations);

				// Retrieve transform post physics step
				std::vector<EntityID> view = m_Registry.GetEntityList<Rigidbody2DComponent>();
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

			tweeningSystem->UpdateTweens(dt);
		}

		SceneCamera* mainCamera = nullptr;
		Mat4 cameraTransform;
		{
			std::vector<EntityID> view = m_Registry.GetEntityList<CameraComponent>();
			for (auto entity : view)
			{
				auto& transformComponent = m_Registry.GetComponent<TransformComponent>(entity);
				auto& cameraComponent = m_Registry.GetComponent<CameraComponent>(entity);

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
				std::vector<EntityID> view = m_Registry.GetEntityList<SpriteRendererComponent>();
				std::sort(view.begin(), view.end(), [&](const EntityID lhs, const EntityID rhs) {
					return m_Registry.GetComponent<TransformComponent>(lhs).m_position.z < m_Registry.GetComponent<TransformComponent>(rhs).m_position.z;
				});

				for (auto entity : view)
				{
					auto spriteRendererComponent = m_Registry.GetComponent<SpriteRendererComponent>(entity);
					auto transformComponent = m_Registry.GetComponent<TransformComponent>(entity);

					Renderer2D::DrawSprite(transformComponent.GetTransform(), spriteRendererComponent, (int)entity);
				}
			}

			// Draw circles
			{
				// To make blending work for multiple objects we have to draw the
				// most distant object first and the closest object last
				std::vector<EntityID> view = m_Registry.GetEntityList<CircleRendererComponent>();
				std::sort(view.begin(), view.end(), [&](const EntityID lhs, const EntityID rhs) {
					return m_Registry.GetComponent<TransformComponent>(lhs).m_position.z < m_Registry.GetComponent<TransformComponent>(rhs).m_position.z;
				});

				for (auto entity : view)
				{
					auto circleRendererComponent = m_Registry.GetComponent<CircleRendererComponent>(entity);
					auto transformComponent = m_Registry.GetComponent<TransformComponent>(entity);

					Renderer2D::DrawCircle(transformComponent.GetTransform(), circleRendererComponent.m_color,
						circleRendererComponent.m_thickness, circleRendererComponent.m_fade, (int)entity);
				}
			}

			OnDraw();

			Renderer2D::EndScene();

			Renderer2D::BeginScene();

			if (Application::Get().GetIsDebugPaused())
			{
				Vec2 position = Vec2(0.9f, 0.9f);
				Ref<Texture2D> texture = Texture2D::Create("Resources/Icons/Pause.png");
				Renderer2D::DrawQuadOverlay(position, 0.0f, 1.0f, texture);
			}

			Renderer2D::EndScene();
		}
	}

	void Scene::OnDraw()
	{
	
	}

	void Scene::OnUpdateEditor(DeltaTime dt, EditorCamera& camera)
	{
		Renderer2D::BeginScene(camera);

		// TODO: Account for all types of rendering when ordering the draw calls

		// Draw Sprites
		{
			// To make blending work for multiple objects we have to draw the
			// most distant object first and the closest object last
			std::vector<EntityID> view = m_Registry.GetEntityList<SpriteRendererComponent>();
			std::sort(view.begin(), view.end(), [&](const EntityID lhs, const EntityID rhs) {
				return m_Registry.GetComponent<TransformComponent>(lhs).m_position.z < m_Registry.GetComponent<TransformComponent>(rhs).m_position.z;
			});

			for (EntityID entity : view)
			{
				if (IsEntityDisabled(entity))
				{
					continue;
				}

				auto spriteRendererComponent = m_Registry.GetComponent<SpriteRendererComponent>(entity);
				auto transformComponent = m_Registry.GetComponent<TransformComponent>(entity);

				Renderer2D::DrawSprite(transformComponent.GetTransform(), spriteRendererComponent, (int)entity);
			}
		}

		// Draw circles
		{
			// To make blending work for multiple objects we have to draw the
			// most distant object first and the closest object last
			std::vector<EntityID> view = m_Registry.GetEntityList<CircleRendererComponent>();
			std::sort(view.begin(), view.end(), [&](const EntityID lhs, const EntityID rhs) {
				return m_Registry.GetComponent<TransformComponent>(lhs).m_position.z < m_Registry.GetComponent<TransformComponent>(rhs).m_position.z;
			});

			for (auto entity : view)
			{
				if (IsEntityDisabled(entity))
				{
					continue;
				}

				auto circleRendererComponent = m_Registry.GetComponent<CircleRendererComponent>(entity);
				auto transformComponent = m_Registry.GetComponent<TransformComponent>(entity);

				Renderer2D::DrawCircle(transformComponent.GetTransform(), circleRendererComponent.m_color,
					circleRendererComponent.m_thickness, circleRendererComponent.m_fade, (int)entity);
			}
		}

		Renderer2D::EndScene();
	}

	void Scene::OnMouseMoved(int x, int y)
	{
		Vec3 cursorCoords = Renderer2D::ConvertScreenToWorldSpace(x, y);
		// Area 2D
		{
			std::vector<EntityID> view = m_Registry.GetEntityList<BoxArea2DComponent>();
			for (auto e : view)
			{
				Entity entity = { e, this };
				auto& transform = entity.GetComponent<TransformComponent>();
				auto& ba2D = entity.GetComponent<BoxArea2DComponent>();

				Vec2 offsetPosition = Vec2(transform.m_position) + ba2D.m_offset;
				bool withinXLimit = (cursorCoords.x < offsetPosition.x + ba2D.m_size.x) && (cursorCoords.x > offsetPosition.x - ba2D.m_size.x);
				bool withinYLimit = (cursorCoords.y < offsetPosition.y + ba2D.m_size.y) && (cursorCoords.y > offsetPosition.y - ba2D.m_size.y);
				if (withinXLimit && withinYLimit)
				{
					if (!ba2D.m_isMouseInArea)
					{
						ba2D.m_isMouseInArea = true;
						ScriptEngine::OnMouseEnterArea(entity);
					}
				}
				else if (ba2D.m_isMouseInArea)
				{
					ba2D.m_isMouseInArea = false;
					ScriptEngine::OnMouseExitArea(entity);
				}
			}
		}
	}

	void Scene::OnMouseButtonPressed(int button)
	{
		{
			std::vector<EntityID> view = m_Registry.GetEntityList<BoxArea2DComponent>();
			for (auto e : view)
			{
				Entity entity = { e, this };
				ScriptEngine::OnMouseButtonPressed(entity, button);
			}
		}
	}

	void Scene::OnMouseButtonReleased(int button)
	{
		{
			std::vector<EntityID> view = m_Registry.GetEntityList<BoxArea2DComponent>();
			for (auto e : view)
			{
				Entity entity = { e, this };
				ScriptEngine::OnMouseButtonReleased(entity, button);
			}
		}
	}

	void Scene::OnViewportResize(uint32_t width, uint32_t height)
	{
		m_ViewportWidth = width;
		m_ViewportHeight = height;

		// Resize our non-fixed aspect ratio cameras
		std::vector<EntityID> view = m_Registry.GetEntityList<CameraComponent>();
		for (auto entity : view)
		{
			auto& cameraComponent = m_Registry.GetComponent<CameraComponent>(entity);
			if (!cameraComponent.GetHasFixedAspectRatio())
			{
				cameraComponent.GetCamera().SetViewportResize(width, height);
			}
		}
	}

	void Scene::SerializeEntity(void* yamlEmitter, Entity entity)
	{

	}

	void Scene::DeserializeEntity(void* yamlEntity, Entity entity)
	{

	}

	void Scene::InitPhyics2D()
	{
		m_PhysicsWorld = new b2World({ 0.0f, -9.8f });

		std::vector<EntityID> view = m_Registry.GetEntityList<Rigidbody2DComponent>();
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

	Ref<Tween> Scene::CreateTween(Entity entity, const TweenParameterStep& tweenStep)
	{
		TweenComponent& tweenComponent = entity.GetOrAddComponent<TweenComponent>();
		Ref<Tween> tween = tweenComponent.CreateTween(tweenStep);
		return tween;
	}

	Ref<Tween> Scene::CreateTween(Entity entity, const TweenCallbackStep& tweenStep)
	{
		TweenComponent& tweenComponent = entity.GetOrAddComponent<TweenComponent>();
		Ref<Tween> tween = tweenComponent.CreateTween(tweenStep);
		return tween;
	}

	Ref<Tween> Scene::CreateTween(Entity entity, const TweenWaitStep& tweenStep)
	{
		TweenComponent& tweenComponent = entity.GetOrAddComponent<TweenComponent>();
		Ref<Tween> tween = tweenComponent.CreateTween(tweenStep);
		return tween;
	}

	Ref<Tween> Scene::CreateTween(Entity entity, float* param, float begin, float finish, float duration, EasingType easingType)
	{
		TweenComponent& tweenComponent = entity.GetOrAddComponent<TweenComponent>();
		Ref<Tween> tween = tweenComponent.CreateTween(param, begin, finish, duration, easingType);
		return tween;
	}

	Ref<Tween> Scene::CreateTween(Entity entity, Vec2* param, Vec2 begin, Vec2 finish, float duration, EasingType easingType)
	{
		TweenComponent& tweenComponent = entity.GetOrAddComponent<TweenComponent>();
		Ref<Tween> tween = tweenComponent.CreateTween(param, begin, finish, duration, easingType);
		return tween;
	}

	Ref<Tween> Scene::CreateTween(Entity entity, Vec3* param, Vec3 begin, Vec3 finish, float duration, EasingType easingType)
	{
		TweenComponent& tweenComponent = entity.GetOrAddComponent<TweenComponent>();
		Ref<Tween> tween = tweenComponent.CreateTween(param, begin, finish, duration, easingType);
		return tween;
	}

	Ref<Tween> Scene::CreateTween(Entity entity, Vec4* param, Vec4 begin, Vec4 finish, float duration, EasingType easingType)
	{
		TweenComponent& tweenComponent = entity.GetOrAddComponent<TweenComponent>();
		Ref<Tween> tween = tweenComponent.CreateTween(param, begin, finish, duration, easingType);
		return tween;
	}

	void Scene::DuplicateEntity(Entity entity)
	{
		std::string name = GetNameForDuplicate(entity);
		Entity newEntity = CreateEntity(name);

		CopyEntityComponents(newEntity, entity);
	}

	void Scene::CopyEntityComponents(Entity dest, Entity src)
	{
		CopyComponentIfExists(RhombusComponents{}, dest, src);
	}

	std::string Scene::GetNameForDuplicate(Entity entity)
	{
		// Look for a number suffix
		std::string digits = "0123456789";
		std::string name = entity.GetName();
		std::string numberSuffix = "";
		size_t currentCharIndex = name.length() - 1;
		char currentChar = name[currentCharIndex];
		while (digits.find(currentChar) != std::string::npos)
		{
			numberSuffix = currentChar + numberSuffix;
			if (currentCharIndex == 0)
			{
				break;
			}
			currentChar = name[--currentCharIndex];
		}

		if (numberSuffix == "")
		{
			// If no number suffic just add 2
			name += " 2";
		}
		else
		{
			// Increment number at end of name
			std::string prefix = name.substr(0, currentCharIndex + 1);
			int intSuffix = std::stoi(numberSuffix);
			intSuffix++;
			name = prefix + std::to_string(intSuffix);

			// If this name already exists than increment the number again
			while (DoesNameExistInScene(name))
			{
				intSuffix++;
				name = prefix + std::to_string(intSuffix);
			}
		}

		return name;
	}

	bool Scene::DoesNameExistInScene(std::string name)
	{
		std::vector<EntityID> view = m_Registry.GetEntityList<TagComponent>();
		for (auto e : view)
		{
			Entity entity = { e, this };
			auto& tag = entity.GetComponent<TagComponent>();
			if (tag.m_tag == name)
			{
				return true;
			}
		}

		return false;
	}

	Entity Scene::GetEntityByUUID(UUID uuid)
	{
		// TODO: Maybe should be assert
		if (m_EntityMap.find(uuid) != m_EntityMap.end())
			return { m_EntityMap.at(uuid), this };

		return {};
	}

	std::vector<EntityID> Scene::GetAllEntities()
	{
		return m_Registry.GetEntityList<IDComponent>();
	}

	Entity Scene::GetPrimaryCameraEntity()
	{
		std::vector<EntityID> view = m_Registry.GetEntityList<CameraComponent>();
		for (auto entity : view)
		{
			auto& camera = m_Registry.GetComponent<CameraComponent>(entity);
			if (camera.GetIsPrimaryCamera())
			{
				return Entity{ entity, this };
			}
		}

		return {};
	}

	bool Scene::IsEntityDisabled(EntityID entity) const
	{
		return m_entityEnabledMap.find(entity) != m_entityEnabledMap.end() && !m_entityEnabledMap.at(entity);
	}
}