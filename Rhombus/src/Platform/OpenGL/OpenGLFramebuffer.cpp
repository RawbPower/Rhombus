#include "rbpch.h"
#include "OpenGLFramebuffer.h"

#include <glad/glad.h>

namespace rhombus
{
	static const uint32_t s_MaxFramebufferSize = 8192;

	namespace utils
	{
		static GLenum TextureTarget(bool multiSampled)
		{
			return multiSampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
		}

		static void CreateTextures(bool multiSampled, uint32_t* outID, uint32_t count)
		{
			glCreateTextures(TextureTarget(multiSampled), count, outID);
		}

		static void BindTexture(bool multiSampled, uint32_t id)
		{
			glBindTexture(TextureTarget(multiSampled), id);
		}

		static void AttachColorTexture(uint32_t id, int samples, GLenum internalFormat, GLenum format, uint32_t width, uint32_t height, int index)
		{
			bool multiSampled = samples > 1;
			if (multiSampled)
			{
				glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, internalFormat, width, height, GL_FALSE);
			}
			else
			{
				glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, nullptr);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			}

			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, TextureTarget(multiSampled), id, 0);
		}

		static void AttachDepthTexture(uint32_t id, int samples, GLenum format, GLenum attachmentType, uint32_t width, uint32_t height)
		{
			bool multiSampled = samples > 1;
			if (multiSampled)
			{
				glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, format, width, height, GL_FALSE);
			}
			else
			{
				glTexStorage2D(GL_TEXTURE_2D, 1, format, width, height);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			}

			glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType, TextureTarget(multiSampled), id, 0);
		}

		static bool IsDepthFormat(FramebufferTextureFormat format)
		{
			switch (format)
			{
				case FramebufferTextureFormat::DEPTH24STENCIL8:		return true;
			}

			return false;
		}

		static GLenum FramebufferTextureFormatToGL(FramebufferTextureFormat format)
		{
			switch (format)
			{
			case FramebufferTextureFormat::RGBA8:       return GL_RGBA8;
			case FramebufferTextureFormat::RED_INTEGER: return GL_RED_INTEGER;
			}

			RB_CORE_ASSERT(false, "Unknown framebuffer texture format.");
			return 0;
		}

	}

	OpenGLFramebuffer::OpenGLFramebuffer(const FramebufferSpecification& spec)
		: m_Specification(spec)
	{
		for (auto texSpec : m_Specification.Attachments.Attachments)
		{
			if (!utils::IsDepthFormat(texSpec.TextureFormat))
				m_colorAttachmentSpecifications.emplace_back(texSpec);
			else
				m_depthAttachmentSpecification = texSpec;
		}

		Invalidate();
	}

	OpenGLFramebuffer::~OpenGLFramebuffer()
	{
		glDeleteFramebuffers(1, &m_RendererID);
		glDeleteTextures((GLsizei)m_colorAttachmentIDs.size(), m_colorAttachmentIDs.data());
		glDeleteTextures((GLsizei)1, &m_depthAttachmentID);
	}

	void OpenGLFramebuffer::Invalidate()
	{
		if (m_RendererID)
		{
			glDeleteFramebuffers(1, &m_RendererID);
			glDeleteTextures((GLsizei)m_colorAttachmentIDs.size(), m_colorAttachmentIDs.data());
			glDeleteTextures((GLsizei)1, &m_depthAttachmentID);

			m_colorAttachmentIDs.clear();
			m_depthAttachmentID = 0;
		}

		glCreateFramebuffers(1, &m_RendererID);
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);

		bool multiSampled = m_Specification.Samples > 1;

		// Attachments
		if (m_colorAttachmentSpecifications.size())
		{
			m_colorAttachmentIDs.resize(m_colorAttachmentSpecifications.size());
			utils::CreateTextures(multiSampled, m_colorAttachmentIDs.data(), (uint32_t)m_colorAttachmentIDs.size());

			for (size_t i = 0; i < m_colorAttachmentIDs.size(); i++)
			{
				utils::BindTexture(multiSampled, m_colorAttachmentIDs[i]);
				switch (m_colorAttachmentSpecifications[i].TextureFormat)
				{
				case FramebufferTextureFormat::RGBA8:
					utils::AttachColorTexture(m_colorAttachmentIDs[i], m_Specification.Samples, GL_RGBA8, GL_RGBA, m_Specification.Width, m_Specification.Height, (int)i);
					break;
				case FramebufferTextureFormat::RED_INTEGER:
					utils::AttachColorTexture(m_colorAttachmentIDs[i], m_Specification.Samples, GL_R32I, GL_RED_INTEGER, m_Specification.Width, m_Specification.Height, (int)i);
					break;
				}
			}
		}

		if (m_depthAttachmentSpecification.TextureFormat != FramebufferTextureFormat::None)
		{
			utils::CreateTextures(multiSampled, &m_depthAttachmentID, 1);
			utils::BindTexture(multiSampled, m_depthAttachmentID);
			switch (m_depthAttachmentSpecification.TextureFormat)
			{
			case FramebufferTextureFormat::DEPTH24STENCIL8:
				utils::AttachDepthTexture(m_depthAttachmentID, m_Specification.Samples, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL_ATTACHMENT, m_Specification.Width, m_Specification.Height);
				break;
			}
		}

		if (m_colorAttachmentSpecifications.size())
		{
			RB_CORE_ASSERT(m_colorAttachmentIDs.size() <= 4, "Exceeded the maximum of 4 color buffers!");
			GLenum buffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
			glDrawBuffers((GLsizei)m_colorAttachmentSpecifications.size(), buffers);
		}
		else if (m_colorAttachmentIDs.empty())
		{
			// Only depth pass
			glDrawBuffer(GL_NONE);
		}

		RB_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete!");

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFramebuffer::Bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
		glViewport(0, 0, m_Specification.Width, m_Specification.Height);
	}

	void OpenGLFramebuffer::Unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFramebuffer::Resize(uint32_t width, uint32_t height)
	{
		if (width == 0 || height == 0 || width > s_MaxFramebufferSize || height > s_MaxFramebufferSize)
		{
			RB_CORE_WARN("Attempted to rezize framebuffer to {0}, {1}", width, height);
			return;
		}

		m_Specification.Width = width;
		m_Specification.Height = height;
		Invalidate();
	}

	int OpenGLFramebuffer::ReadPixel(uint32_t attachmentIndex, int x, int y)
	{
		RB_CORE_ASSERT(attachmentIndex < m_colorAttachmentIDs.size(), "Invalid attachment index!");
		glReadBuffer(GL_COLOR_ATTACHMENT0 + attachmentIndex);
		int pixelData;
		glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pixelData);		// TODO: Bad hardcoding needs fix
		return pixelData;
	}

	void OpenGLFramebuffer::ClearAttachment(uint32_t attachmentIndex, int value)
	{
		RB_CORE_ASSERT(attachmentIndex < m_colorAttachmentIDs.size(), "Invalid color attachment ID!");

		auto& spec = m_colorAttachmentSpecifications[attachmentIndex];

		glClearTexImage(m_colorAttachmentIDs[attachmentIndex], 0, 
			utils::FramebufferTextureFormatToGL(spec.TextureFormat), GL_INT, &value);   // TODO: better way to figure out other non-int types
	}
}