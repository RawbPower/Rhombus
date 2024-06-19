#pragma once

#include "Rhombus/ECS/System.h"
#include "Rhombus/ECS/Scene.h"
#include "Rhombus/Core/DeltaTime.h"
#include "Rhombus/ECS/Entity.h"

namespace rhombus
{
	class CardPlacementSystem : public System
	{
	public:
		CardPlacementSystem(Scene* scene) : System(scene)
		{
		}

		void Init();

		void Update(DeltaTime time);

		void OnMouseMoved(int x, int y);
		void OnMouseButtonPressed(int button);
		void OnMouseButtonReleased(int button);

		void PlaceCard(Entity cardEntity);
		EntityID CheckForCardSlot(Entity cardEntity);
	};
}
