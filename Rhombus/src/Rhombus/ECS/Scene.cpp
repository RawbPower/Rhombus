#include "rbpch.h"
#include "Scene.h"
#include "Component.h"
#include "Entity.h"
#include "Rhombus/Renderer/Renderer2D.h"

#include <glm/glm.hpp>

namespace rhombus
{
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

	void Scene::OnUpdate(DeltaTime dt)
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

		// Render sprites
		Camera* mainCamera = nullptr;
		glm::mat4* cameraTransform = nullptr;
		{
			auto view = m_Registry.view<TransformComponent, CameraComponent>();
			for (auto entity : view)
			{
				auto [transformComponent, cameraComponent] = view.get<TransformComponent, CameraComponent>(entity);

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

			// TODO: Work out Z sorting
			// To make blending work for multiple objects we have to draw the
			// most distant object first and the closest object last
			auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
			for (auto entity : group)
			{
				auto [transformComponent, spriteRendererComponent] = group.get<TransformComponent, SpriteRendererComponent>(entity);

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