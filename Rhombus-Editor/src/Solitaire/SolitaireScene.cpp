#include "SolitaireScene.h"

#include "Components/CardComponent.h"

#include <fstream>

#define YAML_CPP_STATIC_DEFINE		// Needed for yaml static library to work for some reason
#include <yaml-cpp/yaml.h>

namespace rhombus
{
	SolitaireScene::SolitaireScene()
	{

	}

	void SolitaireScene::OnUpdateRuntime(DeltaTime dt)
	{
		Scene::OnUpdateRuntime(dt);
	}

	void SolitaireScene::SerializeEntity(void* yamlEmitter, Entity entity)
	{
		YAML::Emitter& out = *(YAML::Emitter*)yamlEmitter;
		if (entity.HasComponent<CardComponent>())
		{
			out << YAML::Key << "CardComponent";
			out << YAML::BeginMap; // CardComponent

			auto& cardComponent = entity.GetComponent<CardComponent>();
			out << YAML::Key << "Rank" << YAML::Value << cardComponent.m_rank;
			out << YAML::Key << "Suit" << YAML::Value << cardComponent.m_suit;

			out << YAML::EndMap; // CardComponent
		}
	}

	void SolitaireScene::DeserializeEntity(void* yamlEntity, Entity entity)
	{
		YAML::Node node = *(YAML::Node*)yamlEntity;

		auto cardComponent = node["CardComponent"];
		if (cardComponent)
		{
			auto& card = entity.AddComponent<CardComponent>();
			card.m_rank = cardComponent["Rank"].as<int>();
			card.m_suit = cardComponent["Suit"].as<int>();
		}
	}

	void SolitaireScene::OnMouseMoved(int x, int y)
	{
		Scene::OnMouseMoved(x, y);
		glm::vec3 cursorCoords = Renderer2D::ConvertScreenToWorldSpace(x, y);

		{
			auto view = m_Registry.view<CardComponent>();
			for (auto e : view)
			{
				Entity entity = { e, this };
				auto& transform = entity.GetComponent<TransformComponent>();
				auto& card = entity.GetComponent<CardComponent>();

				if (card.GetIsHeld())
				{
					transform.m_position = glm::vec3(cursorCoords.x, cursorCoords.y, transform.m_position.z);
				}
			}
		}
	}

	void SolitaireScene::OnMouseButtonPressed(int button)
	{
		Scene::OnMouseButtonPressed(button);

		{
			auto view = m_Registry.view<CardComponent>();
			for (auto e : view)
			{
				Entity entity = { e, this };
				auto& card = entity.GetComponent<CardComponent>();

				if (entity.HasComponent<BoxArea2DComponent>())
				{
					auto& ba2D = entity.GetComponent<BoxArea2DComponent>();

					if (ba2D.m_isMouseInArea)
					{
						card.SetIsHeld(true);
					}
				}
			}
		}
	}

	void SolitaireScene::OnMouseButtonReleased(int button)
	{
		Scene::OnMouseButtonReleased(button);

		{
			auto view = m_Registry.view<CardComponent>();
			for (auto e : view)
			{
				Entity entity = { e, this };
				auto& card = entity.GetComponent<CardComponent>();

				if (card.GetIsHeld())
				{
					card.SetIsHeld(false);
				}
			}
		}
	}

	template<>
	void Scene::OnComponentAdded<CardComponent>(Entity entity, CardComponent& component)
	{
	}
}