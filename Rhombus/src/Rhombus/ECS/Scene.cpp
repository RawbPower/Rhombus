#include "rbpch.h"
#include "Scene.h"
#include "Component.h"
#include "Entity.h"
#include "Rhombus/Renderer/Renderer2D.h"

#include <glm/glm.hpp>

namespace rhombus
{

	static void OnTransformConstruct(entt::registry& registry, entt::entity entity)
	{

	}

	static std::tuple<int, int, int> foo()
	{

	}

	Scene::Scene()
	{
		/*
		entt::entity entity = m_Registry.create();
		m_Registry.emplace<TransformComponent>(entity, glm::mat4(1.0f));

		TransformComponent& transformCompnent = m_Registry.get<TransformComponent>(entity);

		m_Registry.on_construct<TransformComponent>().connect<&OnTransformConstruct>();

		auto view = m_Registry.view<TransformComponent>();
		for (auto entity : view)
		{
			TransformComponent& viewTransformCompnent = view.get<TransformComponent>(entity);
		}

		auto group = m_Registry.group<TransformComponent>(entt::get<SpriteComponent>);
		for (auto entity : group)
		{
			auto& [groupTransformComponent, groupSpriteComponent] = group.get<TransformComponent, SpriteComponent>(entity);
		}*/
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

	void Scene::OnUpdate(DeltaTime dt)
	{
		// Render sprites
		Camera* mainCamera = nullptr;
		glm::mat4* cameraTransform = nullptr;
		{
			auto view = m_Registry.view<TransformComponent, CameraComponent>();
			for (auto entity : view)
			{
				auto& [transformComponent, cameraComponent] = view.get<TransformComponent, CameraComponent>(entity);

				if (cameraComponent.GetIsPrimaryCamera())
				{
					mainCamera = &cameraComponent.GetCamera();
					cameraTransform = &transformComponent.GetTransform();
					break;
				}
			}
		}

		if (mainCamera)
		{
			Renderer2D::BeginScene(mainCamera->GetProjection(), *cameraTransform);

			auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
			for (auto entity : group)
			{
				auto& [transformComponent, spriteRendererComponent] = group.get<TransformComponent, SpriteRendererComponent>(entity);

				Renderer2D::DrawQuad(transformComponent.GetTransform(), spriteRendererComponent.GetColor());
			}

			Renderer2D::EndScene();
		}
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
}