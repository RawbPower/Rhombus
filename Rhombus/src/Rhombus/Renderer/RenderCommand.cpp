#include "rbpch.h"
#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace rhombus {

	RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI();
}