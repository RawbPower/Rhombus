#include "rbpch.h"
#include "Framebuffer.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLFramebuffer.h"

namespace rhombus
{
	Ref<Framebuffer> Framebuffer::Create(const FramebufferSpecification& spec)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None: RB_CORE_ASSERT(false, "RendererAPI::None is current not supported!"); return nullptr;
			case RendererAPI::API::OpenGL: return std::make_shared<OpenGLFramebuffer>(spec);
		}

		RB_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}