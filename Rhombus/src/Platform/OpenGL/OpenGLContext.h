#pragma once

#include "Rhombus/Renderer/GraphicsContext.h"

// Forward Declaration
struct SDL_Window;

namespace rhombus {

	class OpenGLContext : public GraphicsContext {

	public:
		//Constructor 
		OpenGLContext(SDL_Window* windowHandle);
		virtual void Init() override;
		virtual void SwapBuffers() override;
	private:
		SDL_Window* m_WindowHandle;

	};
}
