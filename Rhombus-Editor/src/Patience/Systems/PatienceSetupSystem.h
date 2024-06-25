#pragma once

#include "Rhombus.h"

class PatienceSetupSystem : public System
{
public:
	PatienceSetupSystem(Scene* scene) : System(scene)
	{
	}

	void Init();
};
