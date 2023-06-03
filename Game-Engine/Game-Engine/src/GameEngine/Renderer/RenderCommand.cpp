#include "gepch.h"
#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace ge {

	RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI();
}