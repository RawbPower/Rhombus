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

		virtual void OnEvent(Event& e) override;

		void Begin();
		void End();

		void BlockEvents(bool block) { m_BlockEvents = block; }
		void SetRhombusThemeColors();
	private:

		bool m_BlockEvents = true;
		float m_Time = 0.0f;
	};
}
