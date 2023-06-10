#include "rbpch.h"
#include "Application.h"

#include "Rhombus/Core/Log.h"

#include "Rhombus/Renderer/Renderer.h"

#include "Input.h"

#include <glfw/glfw3.h>
#include <SDL.h>

namespace rhombus {

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		// Making application a singleton
		RB_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));

		Renderer::Init();

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);
	}

	// Essentially a wrapper
	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
	}

	void Application::PushOverlay(Layer* overlay)
	{
		m_LayerStack.PushOverlay(overlay);
	}

	void Application::OnEvent(Event& e) {

		// Dispatch events
		EventDispatcher dispatcher(e);
		// Tell disptacher "If you see a window close event that dispatch it to this function"
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));

		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(OnWindowResize));

		dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT_FN(OnKeyPressed));

		dispatcher.Dispatch<KeyReleasedEvent>(BIND_EVENT_FN(OnKeyReleased));

		dispatcher.Dispatch<KeyTypedEvent>(BIND_EVENT_FN(OnKeyTyped));

		//RB_CORE_TRACE("{0}", e);

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); )
		{
			(*--it)->OnEvent(e);
			if (e.Handled)
				break;
		}
	}

	void Application::Run() 
	{
		while (m_Running) 
		{
			float time = (float)glfwGetTime();			// Will be in Platform GetTime() in the future
			DeltaTime deltaTime = time - m_LastFrameTime;
			m_LastFrameTime = time;

			if (!m_Minimised)
			{
				for (Layer* layer : m_LayerStack)
					layer->OnUpdate(deltaTime);
			}

			// Render ImGui
			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack)
				layer->OnImGuiRender();
			m_ImGuiLayer->End();

			m_Window->OnUpdate();
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}

	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		if (e.GetWidth() == 0 || e.GetHeight() == 0)
		{
			m_Minimised = true;
			return false;
		}

		m_Minimised = false;

		// Tell Renderer that Frame Buffer has resized
		Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());

		return false;
	}

	bool Application::OnKeyPressed(KeyPressedEvent& e)
	{
		RB_CORE_INFO(e.ToString());

		return true;
	}

	bool Application::OnKeyReleased(KeyReleasedEvent& e)
	{
		RB_CORE_INFO(e.ToString());

		return true;
	}

	bool Application::OnKeyTyped(KeyTypedEvent& e)
	{
		RB_CORE_INFO(e.ToString());

		return true;
	}
}
