#pragma once

#include "Rhombus.h"

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

	void PlaceCard(Entity cardEntity, bool isInSequence);
	EntityID CheckForCardSlot(Entity cardEntity);
};

