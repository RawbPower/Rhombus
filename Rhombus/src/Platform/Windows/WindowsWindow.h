#pragma once

#include "Rhombus/Core/Window.h"
#include "Rhombus/Renderer/GraphicsContext.h"

#include <GLFW/glfw3.h>
#include <SDL.h>

namespace rhombus {

	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowParams& params);
		virtual ~WindowsWindow();

		void OnUpdate() override;

		inline unsigned int GetWidth() const override { return m_Data.Width; }
		inline unsigned int GetHeight() const override { return m_Data.Height; }

		// Window attributes
		inline void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
		void SetVSync(bool enabled) override;
		bool IsVSync() const override;

		inline virtual void* GetNativeWindow() const { return m_Window; }
	private:
		virtual void Init(const WindowParams& params);
		virtual void Shutdown();

		void Init_SDL();
		void Shutdown_SDL();
		void Update_SDL();

		void HandleEvents_SDL();
	private:
		GLFWwindow* m_Window;
		GraphicsContext* m_Context;

		//The window we'll be rendering to
		SDL_Window* m_SDLWindow;
		//The surface contained by the window
		SDL_Surface* m_SDLScreenSurface;
		//The image we will load and show on the screen
		SDL_Surface* m_SDLHelloWorld;

		struct WindowData
		{
			std::string Title;
			unsigned int Width, Height;
			bool VSync;

			EventCallbackFn EventCallback;
		};

		WindowData m_Data;
	};
}
