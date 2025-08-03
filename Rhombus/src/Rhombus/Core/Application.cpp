#include "rbpch.h"
#include "Application.h"

#include "Rhombus/Core/Log.h"
#include "Rhombus/Core/KeyCodes.h"

#include "Rhombus/Renderer/Renderer.h"
#include "Rhombus/Scripting/ScriptEngine.h"
#include "Rhombus/Utils/PlatformUtils.h"

#include "Input.h"

#include <filesystem>

namespace rhombus {

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;

	Application::Application(const ApplicationSpecification& specification)
		: m_Specification(specification)
	{
		RB_PROFILE_FUNCTION();

		// Making application a singleton
		RB_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		// Set working directory here
		if (!m_Specification.workingDirectory.empty())
			std::filesystem::current_path(m_Specification.workingDirectory);

		m_Window = std::unique_ptr<Window>(Window::Create(WindowParams(m_Specification.name, m_Specification.width, m_Specification.height, m_Specification.fullscreen)));
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));
		SetViewport(0.0f, 0.0f, (float)m_Window->GetWidth(), (float)m_Window->GetHeight());

		Renderer::Init();
		ScriptEngine::Init();

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

	void Application::SetViewport(float x, float y, float width, float height)
	{
		m_Viewport.x = x;
		m_Viewport.y = y;
		m_Viewport.width = width;
		m_Viewport.height = height;
	}

	void Application::Close()
	{
		m_Running = false;
	}

	void Application::OnEvent(Event& e) 
	{
		RB_PROFILE_FUNCTION();

		if (GetImGuiLayer()->ShouldBlockEvents())
		{
			bool deletePressed = Input::IsKeyPressed(RB_KEY_DELETE);
			GetImGuiLayer()->BlockEvents(!deletePressed);
		}

		// Dispatch events
		EventDispatcher dispatcher(e);
		// Tell disptacher "If you see a window close event that dispatch it to this function"
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));

		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(OnWindowResize));

		dispatcher.Dispatch<WindowMovedEvent>(BIND_EVENT_FN(OnWindowMoved));

		dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT_FN(OnKeyPressed));

		dispatcher.Dispatch<KeyReleasedEvent>(BIND_EVENT_FN(OnKeyReleased));

		dispatcher.Dispatch<GamepadAxisEvent>(BIND_EVENT_FN(OnGamepadAxis));

		dispatcher.Dispatch<GamepadButtonDownEvent>(BIND_EVENT_FN(OnGamepadButtonDown));

		dispatcher.Dispatch<GamepadButtonUpEvent>(BIND_EVENT_FN(OnGamepadButtonUp));

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

			// When these is a long timestep (caused by breakpoint)
			// then just artifically set the fps to the target
			static float TARGET_FPS = 60.0f;
			if (deltaTime > 1.0f)
			{
				deltaTime = (1.0f / TARGET_FPS);
			}

			m_LastFrameTime = time;

			if (m_DebugStep)
			{
				// If paused then unpause for a frame
				if (m_DebugPause)
				{
					m_DebugPause = false;
				}
				// If not paused, we have just stepped forward a frame. So pause again and disable step
				else
				{
					m_DebugPause = true;
					m_DebugStep = false;
				}
			}

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

		// Skip time spent resizing the window since the game wasn't running
		m_LastFrameTime = Time::GetTime();

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

	bool Application::OnWindowMoved(WindowMovedEvent& e)
	{
		// Skip time spent moving the window since the game wasn't running
		m_LastFrameTime = Time::GetTime();

		return false;
	}

	bool Application::OnKeyPressed(KeyPressedEvent& e)
	{
		if (e.GetKeyCode() == RB_KEY_LEFT_BRACKET && !m_DebugStep)	// Can't pause while stepping
		{
			m_DebugPause = !m_DebugPause;
		}
		
		if (m_DebugPause)
		{
			if (e.GetKeyCode() == RB_KEY_RIGHT_BRACKET)
			{
				m_DebugStep = true;
			}
		}

		return false;
	}

	bool Application::OnKeyReleased(KeyReleasedEvent& e)
	{
		return false;
	}

	bool Application::OnKeyTyped(KeyTypedEvent& e)
	{
		return false;
	}

	bool Application::OnMouseButtonPressed(MouseButtonPressedEvent& e)
	{
		return true;
	}

	bool Application::OnMouseButtonReleased(MouseButtonReleasedEvent& e)
	{
		return true;
	}

	bool Application::OnMouseScrolled(MouseScrolledEvent& e)
	{
		return true;
	}

	bool Application::OnMouseMoved(MouseMovedEvent& e)
	{
		return true;
	}

	bool Application::OnGamepadAxis(GamepadAxisEvent& e)
	{
		return true;
	}

	bool Application::OnGamepadButtonDown(GamepadButtonDownEvent& e)
	{
		return false;
	}

	bool Application::OnGamepadButtonUp(GamepadButtonUpEvent& e)
	{
		return false;
	}
}
