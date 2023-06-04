#pragma once

#include "GameEngine/Core/Core.h"
#include "GameEngine/Events/Event.h"
#include "GameEngine/Core/DeltaTime.h"

namespace Rhombus {

	class RB_API Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer() = default;

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate(DeltaTime dt) {}
		virtual void OnImGuiRender() {}
		virtual void OnEvent(Event& event) {}

		inline const std::string& GetName() const { return m_DebugName; }
	protected:
		std::string m_DebugName;
	};
}
