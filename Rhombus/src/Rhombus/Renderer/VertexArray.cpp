#include "rbpch.h"
#include "VertexArray.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace rhombus {

	Ref<VertexArray> VertexArray::Create()
	{
		switch (Renderer::GetAPI()) 
		{
			case RendererAPI::API::None:		RB_CORE_ASSERT(false, "RendererAPI::None is currently not supported"); return nullptr;

			case RendererAPI::API::OpenGL:	return std::make_shared<OpenGLVertexArray>();
		}

		RB_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}
}