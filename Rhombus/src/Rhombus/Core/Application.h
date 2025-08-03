#pragma once

#include "Core.h"

#include "Window.h"
#include "Rhombus/Core/LayerStack.h"
#include "Rhombus/Events/Event.h"
#include "Rhombus/Events/ApplicationEvent.h"

#include "Rhombus/Events/KeyEvent.h"
#include "Rhombus/Events/MouseEvent.h"
#include "Rhombus/Events/GamepadEvent.h"

#include "Rhombus/Core/DeltaTime.h"

#include "Rhombus/ImGui/ImGuiLayer.h"

int main(int argc, char** argv);

namespace rhombus {

	struct ApplicationCommandLineArgs
	{
		int count = 0;
		char** args = nullptr;

		const char* operator[](int index) const
		{
			RB_CORE_ASSERT(index < count, "Index out of range");
			return args[index];
		}
	};

	struct ApplicationSpecification
	{
		std::string name = "Rhombus Application";
		std::string workingDirectory;
		std::string engineDirectory = "../Rhombus";
		std::string editorDirectory = "../Rhombus-Editor";
		ApplicationCommandLineArgs commandLineArgs;
		uint32_t width;
		uint32_t height;
		bool fullscreen = false;
	};

	struct Viewport
	{
		float x = 0.0f;
		float y = 0.0f;
		float width = 0.0f;
		float height = 0.0f;
	};

	class Application
	{
	public:
		Application(const ApplicationSpecification& specification);
		virtual ~Application() = default;

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		inline Window& GetWindow() { return *m_Window; }

		Viewport GetViewport() const { return m_Viewport; }
		void SetViewport(float x, float y, float width, float height);

		void Close();

		ImGuiLayer* GetImGuiLayer() { return m_ImGuiLayer; }

		inline static Application& Get() { return *s_Instance; }

		const ApplicationSpecification& GetSpecification() const { return m_Specification; }

		std::string GetPathRelativeToEngineDirectory(const std::string& path) const { return m_Specification.engineDirectory + "/" + path; }
		std::string GetPathRelativeToEditorDirectory(const std::string& path) const { return m_Specification.editorDirectory + "/" + path; }

		bool GetIsDebugPaused() const { return m_DebugPause; }
	private:
		void Run();

		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);
		bool OnWindowMoved(WindowMovedEvent& e);

		bool OnKeyPressed(KeyPressedEvent& e);
		bool OnKeyReleased(KeyReleasedEvent& e);
		bool OnKeyTyped(KeyTypedEvent& e);

		bool OnMouseButtonPressed(MouseButtonPressedEvent& e);
		bool OnMouseButtonReleased(MouseButtonReleasedEvent& e);
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnMouseMoved(MouseMovedEvent& e);

		bool OnGamepadAxis(GamepadAxisEvent& e);
		bool OnGamepadButtonDown(GamepadButtonDownEvent& e);
		bool OnGamepadButtonUp(GamepadButtonUpEvent& e);

	private:
		ApplicationSpecification m_Specification;
		Scope<Window> m_Window;
		Viewport m_Viewport;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		bool m_Minimised = false;
		bool m_DebugPause = false;
		bool m_DebugStep = false;
		LayerStack m_LayerStack;
		float m_LastFrameTime = 0.0f;
	private:
		static Application* s_Instance;
		friend int ::main(int argc, char** argv);
	};

	// To be defined in CLIENT
	Application* CreateApplication(ApplicationCommandLineArgs args);

}

