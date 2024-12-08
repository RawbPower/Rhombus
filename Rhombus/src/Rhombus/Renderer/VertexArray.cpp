#include "rbpch.h"
#include "VertexArray.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace rhombus {

	Ref<VertexArray> VertexArray::Create()
	{
		switch (Renderer::GetAPI()) 
		{
			case RendererAPI::API::None:		Log::Assert(false, "RendererAPI::None is currently not supported"); return nullptr;

			case RendererAPI::API::OpenGL:	return std::make_shared<OpenGLVertexArray>();
		}

		Log::Assert(false, "Unknown RendererAPI");
		return nullptr;
	}
}