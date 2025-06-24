#pragma once

#include "Rhombus.h"

#include "Systems/PlayerInputSystem.h"

class GameScene : public Scene
{
public:
	GameScene();

	virtual void InitScene() override;

	virtual void OnRuntimeStart() override;
	virtual void OnUpdateRuntime(DeltaTime dt) override;
	virtual void OnDraw() override;

	virtual void CopyAllComponents(Ref<Scene> destScene, const std::unordered_map<UUID, EntityID>& entityMap) override;
	virtual void CopyEntityComponents(Entity dest, Entity src) override;

	virtual void SerializeEntity(void* yamlEmitter, Entity entity) override;
	virtual void DeserializeEntity(void* yamlEntity, Entity entity) override;

	virtual void OnMouseMoved(int x, int y) override;
	virtual void OnMouseButtonPressed(int button) override;
	virtual void OnMouseButtonReleased(int button) override;

private:
	// Systems
	Ref<PlayerInputSystem> playerInputSystem;
};
