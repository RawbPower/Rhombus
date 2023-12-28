#include "rbpch.h"
#include "Scene.h"
#include "Component.h"
#include "Entity.h"
#include "Rhombus/Renderer/Renderer2D.h"

#include <glm/glm.hpp>

// Box2d
#include <box2d/b2_world.h>
#include <box2d/b2_body.h>
#include <box2d/b2_fixture.h>
#include <box2d/b2_polygon_shape.h>

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

	}

	Entity Scene::CreateEntity(const std::string& name)
	{
		Entity entity(m_Registry.create(), this);
		entity.AddComponent<TransformComponent>();
		auto& tag = entity.AddComponent<TagComponent>();
		tag.m_tag = name.empty() ? "Entity" : name;
		return entity;
	}

	void Scene::DestroyEntity(Entity entity)
	{
		m_Registry.destroy(entity);
	}

	void Scene::OnRuntimeStart()
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
				body->CreateFixture(&	fixtureDef);
			}
		}
	}

	void Scene::OnRuntimeStop()
	{
		delete m_PhysicsWorld;
		m_PhysicsWorld = nullptr;
	}

	void Scene::OnUpdateRuntime(DeltaTime dt)
	{
		// Update Scripts
		{
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

			Renderer2D::EndScene();
		}
	}

	void Scene::OnUpdateEditor(DeltaTime dt, EditorCamera& camera)
	{
		Renderer2D::BeginScene(camera);

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
		static_assert(false);
	}

	template<>
	void Scene::OnComponentAdded<TransformComponent>(Entity entity, TransformComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<CameraComponent>(Entity entity, CameraComponent& component)
	{
		component.GetCamera().SetViewportResize(m_ViewportWidth, m_ViewportHeight);
	}

	template<>
	void Scene::OnComponentAdded<TagComponent>(Entity entity, TagComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<SpriteRendererComponent>(Entity entity, SpriteRendererComponent& component)
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
}