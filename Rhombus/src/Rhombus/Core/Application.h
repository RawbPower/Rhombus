#pragma once

#include "Core.h"

#include "Window.h"
#include "Rhombus/Core/LayerStack.h"
#include "Rhombus/Events/Event.h"
#include "Rhombus/Events/ApplicationEvent.h"

#include "Rhombus/Core/DeltaTime.h"

#include "Rhombus/ImGui/ImGuiLayer.h"

namespace rhombus {

	class Application
	{
	public:
		Application();
		virtual ~Application() = default;

		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		inline Window& GetWindow() { return *m_Window; }

		inline static Application& Get() { return *s_Instance; }
	private:
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);

		void HandleEvents_SDL();
	private:
		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		bool m_Minimised = false;
		LayerStack m_LayerStack;
		float m_LastFrameTime = 0.0f;
	private:
		static Application* s_Instance;
	};

	// To be defined in CLIENT
	Application* CreateApplication();

}

