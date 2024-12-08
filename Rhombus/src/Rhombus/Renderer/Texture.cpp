#include "rbpch.h"
#include "Texture.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLTexture.h"

namespace rhombus {

	Ref<Texture2D> Texture2D::Create(uint32_t width, uint32_t height)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:		Log::Assert(false, "RendererAPI::None is currently not supported"); return nullptr;

		case RendererAPI::API::OpenGL:	return std::make_shared<OpenGLTexture2D>(width, height);
		}

		Log::Assert(false, "Unknown RendererAPI");
		return nullptr;
	}

	Ref<Texture2D> Texture2D::Create(const std::string& path) 
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:		Log::Assert(false, "RendererAPI::None is currently not supported"); return nullptr;

			case RendererAPI::API::OpenGL:	return std::make_shared<OpenGLTexture2D>(path);
		}

		Log::Assert(false, "Unknown RendererAPI");
		return nullptr;
	}
}