#pragma once

#include "GameEngine/Renderer/GraphicsContext.h"

// Forward Declaration
struct GLFWwindow;

namespace Rhombus {

	class OpenGLContext : public GraphicsContext {

	public:
		//Constructor 
		OpenGLContext(GLFWwindow* windowHandle);
		virtual void Init() override;
		virtual void SwapBuffers() override;
	private:
		GLFWwindow* m_WindowHandle;

	};
}
