#include "rbpch.h"
#include "OpenGLContext.h"

#include <glad/glad.h>
#include <SDL.h>
#include <SDL_opengl.h>

namespace rhombus {

	OpenGLContext::OpenGLContext(SDL_Window* windowHandle)
		: m_WindowHandle(windowHandle)
	{
		RB_CORE_ASSERT(m_WindowHandle, "Window handle is null!");
	}

	void OpenGLContext::Init() 
	{
		RB_PROFILE_FUNCTION();

		//Create context
		SDL_GLContext glContext = SDL_GL_CreateContext(m_WindowHandle);
		if (glContext == NULL)
			RB_CORE_ERROR("Failed to create OpenGL context");

		int status = gladLoadGLLoader(SDL_GL_GetProcAddress);
		RB_CORE_ASSERT(status, "Failed to initialize Glad!");

		// Check what gpu driver you have
		RB_CORE_INFO("OpenGL Info:");
		RB_CORE_INFO("  Vendor: {0}", glGetString(GL_VENDOR));
		RB_CORE_INFO("  Renderer: {0}", glGetString(GL_RENDERER));
		RB_CORE_INFO("  Version: {0}", glGetString(GL_VERSION));

#ifdef RB_ENABLE_ASSERTS
		int versionMajor;
		int versionMinor;
		glGetIntegerv(GL_MAJOR_VERSION, &versionMajor);
		glGetIntegerv(GL_MINOR_VERSION, &versionMinor);

		RB_CORE_ASSERT(versionMajor > 4 || (versionMajor == 4 && versionMinor >= 5), "Rhombus requires at least OpenGL version 4.5!");
#endif

		//Use Vsync
		if (SDL_GL_SetSwapInterval(1) < 0)
		{
			RB_CORE_ERROR("Warning: Unable to set VSync! SDL Error: {0}\n", SDL_GetError());
		}

		// Disable depth test and face culling.
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);
		glDisable(GL_SCISSOR_TEST);
	}

	void OpenGLContext::SwapBuffers() 
	{
		RB_PROFILE_FUNCTION();

		SDL_GL_SwapWindow(m_WindowHandle);
	}
}