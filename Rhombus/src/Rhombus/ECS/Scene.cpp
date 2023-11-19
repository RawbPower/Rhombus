#include "rbpch.h"
#include "Scene.h"
#include "Component.h"
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

	entt::entity Scene::CreateEntity()
	{
		return m_Registry.create();
	}

	void Scene::OnUpdate(DeltaTime dt)
	{
		auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
		for (auto entity : group)
		{
			auto& [transformComponent, spriteRendererComponent] = group.get<TransformComponent, SpriteRendererComponent>(entity);

			Renderer2D::DrawQuad(transformComponent.GetTransform(), spriteRendererComponent.GetColor());
		}
	}
}