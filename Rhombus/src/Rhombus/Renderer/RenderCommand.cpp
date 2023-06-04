#include "rbpch.h"
#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Rhombus {

	RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI();
}