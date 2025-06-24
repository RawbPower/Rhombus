#pragma once

#include "Rhombus.h"

class PlayerInputSystem : public System
{
public:
	PlayerInputSystem(Scene* scene) : System(scene)
	{
	}

	void Update();
};

