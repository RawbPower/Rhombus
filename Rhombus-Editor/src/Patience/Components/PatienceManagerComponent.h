#pragma once

#include "Rhombus.h"

namespace rhombus
{
	class PatienceManagerComponent : public ComponentBase
	{
	public:
		PatienceManagerComponent() = default;
		PatienceManagerComponent(const PatienceManagerComponent&) = default;

		std::string m_setupScript;
	};
}
