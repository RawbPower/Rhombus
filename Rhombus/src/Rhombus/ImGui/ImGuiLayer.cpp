// Will probably have a different imgui layer for wach platform

#include "rbpch.h"
#include "ImGuiLayer.h"

#include "imgui.h"
#include "backends/imgui_impl_opengl3.h"
#include "backends/imgui_impl_sdl2.h"

#include "Rhombus/Core/Application.h"

// TEMPORARY
#include <SDL.h>
#include <glad/glad.h>

#include "ImGuizmo.h"

namespace rhombus {

	ImGuiLayer::ImGuiLayer() 
		: Layer("ImGuiLayer")
	{

	}

	void ImGuiLayer::OnAttach()
	{
		RB_PROFILE_FUNCTION();

		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		//io.Fonts->AddFontFromFileTTF("ProggyClean.ttf", 12);
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoTaskBarIcons;
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoMerge;

		io.FontDefault = io.Fonts->AddFontFromFileTTF(Application::Get().GetPathRelativeToEngineDirectory("resources/fonts/ProggyClean.ttf").c_str(), 13.0f);

		// Setup Dear ImGui
		//ImGui::StyleColorsDark();
		ImGui::StyleColorsClassic();

		// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		SetRhombusThemeColors();

		Application& app = Application::Get();
		SDL_Window* window = static_cast<SDL_Window*>(app.GetWindow().GetNativeWindow());
		SDL_GLContext context = static_cast<SDL_GLContext*>(app.GetWindow().GetNativeContext());

		// Setup Platform/Renderer bindings
		ImGui_ImplSDL2_InitForOpenGL(window, context);
		ImGui_ImplOpenGL3_Init("#version 410");
	}

	void ImGuiLayer::OnDetach()
	{
		RB_PROFILE_FUNCTION();

		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplSDL2_Shutdown();
		ImGui::DestroyContext();
	}

	void ImGuiLayer::OnEvent(Event& e)
	{
		if (m_BlockEvents)
		{
			ImGuiIO& io = ImGui::GetIO();
			e.Handled |= e.IsInCategory(EventCategoryMouse) & io.WantCaptureMouse;
			e.Handled |= e.IsInCategory(EventCategoryKeyboard) & io.WantCaptureKeyboard;
		}
	}

	void ImGuiLayer::Begin() 
	{
		RB_PROFILE_FUNCTION();

		Application& app = Application::Get();
		SDL_Window* window = static_cast<SDL_Window*>(app.GetWindow().GetNativeWindow());

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL2_NewFrame(window);
		ImGui::NewFrame();
		ImGuizmo::BeginFrame();
	}

	void ImGuiLayer::End()
	{
		RB_PROFILE_FUNCTION();

		ImGuiIO& io = ImGui::GetIO();
		Application& app = Application::Get();
		io.DisplaySize = ImVec2((float)app.GetWindow().GetWidth(), (float)app.GetWindow().GetHeight());

		// Rendering
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			SDL_Window* backup_current_window = SDL_GL_GetCurrentWindow();
			SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();

			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			SDL_GL_MakeCurrent(backup_current_window, backup_current_context);
		}
	}

	void ImGuiLayer::SetRhombusThemeColors()
	{
		auto& colors = ImGui::GetStyle().Colors;

		ImVec4 orange{ 0.764f, 0.427f, 0.267f , 1.0f };
		ImVec4 orangeLight{ 0.890f, 0.580f, 0.329f , 1.0f };
		ImVec4 orangeLight2{ 0.9059f, 0.690f, 0.4235f , 1.0f };
		ImVec4 orangeDark{ 0.6745f, 0.3137f, 0.2627f , 1.0f };

		ImVec4 wine{ 0.376f, 0.145f, 0.2784f , 1.0f };
		ImVec4 purple{ 0.2156f, 0.102f, 0.2784f , 1.0f };
		ImVec4 yellow{ 0.9686f, 0.925f, 0.682f , 1.0f };

		colors[ImGuiCol_WindowBg] = ImVec4{ 0.165f, 0.155f, 0.15f, 1.0f };
		colors[ImGuiCol_MenuBarBg] = purple;

		// Headers
		colors[ImGuiCol_Header] = orangeDark;
		colors[ImGuiCol_HeaderHovered] = orangeLight;
		colors[ImGuiCol_HeaderActive] = orangeLight2;

		// Buttons
		colors[ImGuiCol_Button] = orangeDark;
		colors[ImGuiCol_ButtonHovered] = orangeLight;
		colors[ImGuiCol_ButtonActive] = orangeLight2;

		// Frame BG
		colors[ImGuiCol_FrameBg] = orange;
		colors[ImGuiCol_FrameBgHovered] = orangeLight;
		colors[ImGuiCol_FrameBgActive] = orangeLight2;

		// Tabs
		colors[ImGuiCol_Tab] = orangeLight2;
		colors[ImGuiCol_TabHovered] = orange;
		colors[ImGuiCol_TabActive] = orangeDark;
		colors[ImGuiCol_TabUnfocused] = orangeLight2;
		colors[ImGuiCol_TabUnfocusedActive] = orangeDark;

		// Title
		colors[ImGuiCol_TitleBg] = orange;
		colors[ImGuiCol_TitleBgActive] = orange;
		colors[ImGuiCol_TitleBgCollapsed] = orange;

		// Scrollbar
		colors[ImGuiCol_ScrollbarGrab] = orange;
		colors[ImGuiCol_ScrollbarGrabHovered] = orange;
		colors[ImGuiCol_ScrollbarGrabActive] = orangeLight2;
	}
}