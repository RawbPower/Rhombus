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
		void Update(DeltaTime time);

		void OnMouseMoved(int x, int y, Scene& srcScene);
		void OnMouseButtonPressed(int button, Scene& srcScene);
		void OnMouseButtonReleased(int button, Scene& srcScene);

		void PlaceCard(Entity cardEntity, Scene& srcScene);
	};
}
