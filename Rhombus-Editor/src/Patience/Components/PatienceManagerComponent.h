#pragma once

#include "Rhombus.h"

class PatienceManagerComponent : public ComponentBase
{
public:
	PatienceManagerComponent() = default;
	PatienceManagerComponent(const PatienceManagerComponent&) = default;

	std::string m_setupScript;
};
