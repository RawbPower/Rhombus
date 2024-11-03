#include "GameScene.h"

#include "Rhombus/Scenes/SceneSerializer.h"

// Update this list when any new components are added
// -----------------------------------------------------
using GameComponents =
	ComponentGroup<>;
// -----------------------------------------------------

GameScene::GameScene()
{
	InitScene();
}

void GameScene::InitScene()
{
	Scene::InitScene();

}

void GameScene::OnRuntimeStart()
{
	Scene::OnRuntimeStart();
}

void GameScene::OnUpdateRuntime(DeltaTime dt)
{
	Scene::OnUpdateRuntime(dt);
}

void GameScene::OnDraw()
{
}

void GameScene::CopyAllComponents(Ref<Scene> destScene, const std::unordered_map<UUID, EntityID>& entityMap)
{
	Scene::CopyAllComponents(destScene, entityMap);
	CopyComponent(GameComponents{}, destScene, m_Registry, entityMap);
}

void GameScene::CopyEntityComponents(Entity dest, Entity src)
{
	Scene::CopyEntityComponents(dest, src);
	CopyComponentIfExists(GameComponents{}, dest, src);
}

void GameScene::SerializeEntity(void* yamlEmitter, Entity entity)
{
}

void GameScene::DeserializeEntity(void* yamlEntity, Entity entity)
{
}

void GameScene::OnMouseMoved(int x, int y)
{
	Scene::OnMouseMoved(x, y);
}

void GameScene::OnMouseButtonPressed(int button)
{
	Scene::OnMouseButtonPressed(button);
}

void GameScene::OnMouseButtonReleased(int button)
{
	Scene::OnMouseButtonReleased(button);
}