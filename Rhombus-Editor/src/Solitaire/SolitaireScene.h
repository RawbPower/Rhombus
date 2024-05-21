#pragma once

#include "Rhombus.h"

namespace rhombus
{
	class Entity;

	class SolitaireScene : public Scene
	{
	public:
		SolitaireScene();

		virtual void OnUpdateRuntime(DeltaTime dt) override;

		virtual void SerializeEntity(void* yamlEmitter, Entity entity) override;
		virtual void DeserializeEntity(void* yamlEntity, Entity entity) override;

		virtual void OnMouseMoved(int x, int y) override;
		virtual void OnMouseButtonPressed(int button) override;
		virtual void OnMouseButtonReleased(int button) override;
	};
}
