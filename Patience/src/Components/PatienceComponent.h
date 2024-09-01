#pragma once

#include "Rhombus.h"

class PatienceComponent : public ComponentBase
{
public:
	PatienceComponent() = default;
	PatienceComponent(const PatienceComponent&) = default;

	std::string m_setupScript;
};
