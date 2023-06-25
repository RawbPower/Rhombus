#pragma once

#include "Rhombus/Core/Layer.h"

#include "Rhombus/Events/ApplicationEvent.h"
#include "Rhombus/Events/KeyEvent.h"
#include "Rhombus/Events/MouseEvent.h"

namespace rhombus {

	class RB_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		void Begin();
		void End();
	private:
		float m_Time = 0.0f;
	};
}
