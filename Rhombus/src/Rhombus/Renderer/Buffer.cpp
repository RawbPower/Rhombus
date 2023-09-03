#include "rbpch.h"
#include "Buffer.h"
#include "Renderer.h"

#include "Platform/OpenGL/OpenGLBuffer.h"

namespace rhombus {

	VertexBuffer* VertexBuffer::Create(float* vertices, uint32_t size)
	{
		switch (Renderer::GetAPI()) 
		{
			case RendererAPI::API::None:		RB_CORE_ASSERT(false, "RendererAPI::None is currently not supported"); return nullptr;

			case RendererAPI::API::OpenGL:	return new OpenGLVertexBuffer(vertices, size);
		}

		RB_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}

	VertexBuffer* VertexBuffer::Create(uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:		RB_CORE_ASSERT(false, "RendererAPI::None is currently not supported"); return nullptr;

			case RendererAPI::API::OpenGL:	return new OpenGLVertexBuffer(size);
		}

		RB_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}

	IndexBuffer* IndexBuffer::Create(uint32_t* indices, uint32_t count)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:		RB_CORE_ASSERT(false, "RendererAPI::None is currently not supported"); return nullptr;

			case RendererAPI::API::OpenGL:	return new OpenGLIndexBuffer(indices, count);
		}

		RB_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}
}