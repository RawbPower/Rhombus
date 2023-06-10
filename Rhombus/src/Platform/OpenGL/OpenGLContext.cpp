#include "rbpch.h"
#include "OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <GL/GL.h>

namespace rhombus {

	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		: m_WindowHandle(windowHandle)
	{
		RB_CORE_ASSERT(m_WindowHandle, "Window handle is null!");
	}

	void OpenGLContext::Init() 
	{
		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		RB_CORE_ASSERT(status, "Failed to initialize Glad!");

		// Check what gpu driver you have
		RB_CORE_INFO("OpenGL Info:");
		RB_CORE_INFO("  Vendor: {0}", glGetString(GL_VENDOR));
		RB_CORE_INFO("  Renderer: {0}", glGetString(GL_RENDERER));
		RB_CORE_INFO("  Version: {0}", glGetString(GL_VERSION));
	}

	void OpenGLContext::SwapBuffers() 
	{
		glfwSwapBuffers(m_WindowHandle);
	}
}