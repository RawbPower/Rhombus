#pragma once

#include "Rhombus/Core/Window.h"
#include "Rhombus/Renderer/GraphicsContext.h"

#include <SDL.h>
#include <SDL_opengl.h>

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
		//void SetVSync(bool enabled) override;
		bool IsVSync() const override;

		inline virtual void* GetNativeWindow() const { return m_Window; }
		inline virtual void* GetNativeContext() const { return m_SDLContext; }
	private:
		virtual void Init(const WindowParams& params);
		virtual void Shutdown();

		void HandleEvents();
	private:
		GraphicsContext* m_Context;

		SDL_Surface* m_IconSurface;

		//The window we'll be rendering to
		SDL_Window* m_Window;
		SDL_GLContext m_SDLContext;

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
