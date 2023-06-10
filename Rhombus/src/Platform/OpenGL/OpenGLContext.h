#pragma once

#include "Rhombus/Renderer/GraphicsContext.h"

// Forward Declaration
struct GLFWwindow;

namespace rhombus {

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
