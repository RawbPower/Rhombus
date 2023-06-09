#pragma once

#include "rbpch.h"

#include "Rhombus/Core/Core.h"
#include "Rhombus/Events/Event.h"

namespace rhombus {

	struct WindowParams
	{
		std::string Title;
		unsigned int Width;
		unsigned int Height;

		WindowParams(const std::string& title = "Game Engine",
			unsigned int width = 960,
			unsigned int height = 540)
			: Title(title), Width(width), Height(height)
		{

		}
	};

	// Interface representing a desktop system based Window
	class RB_API Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window() = default;

		virtual void OnUpdate() = 0;

		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;

		// Window attributes
		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		//virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		virtual void* GetNativeWindow() const = 0;
		virtual void* GetNativeContext() const = 0;

		// This get called depending on Platform
		static Window* Create(const WindowParams& params = WindowParams());
	};
}
