#include "rbpch.h"
#include "Application.h"

#include "Rhombus/Core/Log.h"

#include "Rhombus/Renderer/Renderer.h"
#include "Rhombus/Utils/PlatformUtils.h"

#include "Input.h"

namespace rhombus {

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;

	Application::Application(const std::string& name)
	{
		RB_PROFILE_FUNCTION();

		// Making application a singleton
		RB_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create(WindowParams(name)));
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));

		Renderer::Init();

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);
	}

	// Look into this
	/*Application::~Application()
	{
		RB_PROFILE_FUNCTION();

		Renderer::Shutdown();
	}*/

	// Essentially a wrapper
	void Application::PushLayer(Layer* layer)
	{
		RB_PROFILE_FUNCTION();

		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* overlay)
	{
		RB_PROFILE_FUNCTION();

		m_LayerStack.PushOverlay(overlay);
		overlay->OnAttach();
	}

	void Application::Close()
	{
		m_Running = false;
	}

	void Application::OnEvent(Event& e) 
	{
		RB_PROFILE_FUNCTION();

		// Dispatch events
		EventDispatcher dispatcher(e);
		// Tell disptacher "If you see a window close event that dispatch it to this function"
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));

		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(OnWindowResize));

		//dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT_FN(OnKeyPressed));

		//dispatcher.Dispatch<KeyReleasedEvent>(BIND_EVENT_FN(OnKeyReleased));

		//dispatcher.Dispatch<MouseButtonPressedEvent>(BIND_EVENT_FN(OnMouseButtonPressed));

		//dispatcher.Dispatch<MouseButtonReleasedEvent>(BIND_EVENT_FN(OnMouseButtonReleased));

		//dispatcher.Dispatch<KeyTypedEvent>(BIND_EVENT_FN(OnKeyTyped));

		//RB_CORE_TRACE("{0}", e);

		for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it)
		{
			(*it)->OnEvent(e);
			if (e.Handled)
				break;
		}
	}

	void Application::Run() 
	{
		RB_PROFILE_FUNCTION();

		while (m_Running) 
		{
			RB_PROFILE_SCOPE("Run Loop");

			float time = Time::GetTime();
			DeltaTime deltaTime = time - m_LastFrameTime;
			m_LastFrameTime = time;

			if (!m_Minimised)
			{
				{
					RB_PROFILE_SCOPE("LayerStack OnUpdates");

					for (Layer* layer : m_LayerStack)
						layer->OnUpdate(deltaTime);
				}

				// Render ImGui
				m_ImGuiLayer->Begin();
				{
					RB_PROFILE_SCOPE("LayerStack OnImGuiRender");

					for (Layer* layer : m_LayerStack)
						layer->OnImGuiRender();
				}
				m_ImGuiLayer->End();
			}

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
		RB_PROFILE_FUNCTION();

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

	bool Application::OnMouseButtonPressed(MouseButtonPressedEvent& e)
	{
		RB_CORE_INFO(e.ToString());

		return true;
	}

	bool Application::OnMouseButtonReleased(MouseButtonReleasedEvent& e)
	{
		RB_CORE_INFO(e.ToString());

		return true;
	}

	bool Application::OnMouseScrolled(MouseScrolledEvent& e)
	{
		RB_CORE_INFO(e.ToString());

		return true;
	}

	bool Application::OnMouseMoved(MouseMovedEvent& e)
	{
		RB_CORE_INFO(e.ToString());

		return true;
	}
}
