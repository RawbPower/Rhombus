#pragma once

#include "VertexArray.h"

#include "Rhombus/Core/Color.h"

namespace rhombus {

	// There will be an implementation of this class for each platform (ie. OpenGL, DirectX, Vulcan etc.)
	class RendererAPI {
	public:
		enum class API {
			None = 0, OpenGL = 1
		};
	public:
		virtual ~RendererAPI() = default;

		virtual void Init() = 0;
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;

		virtual void SetClearColor(const Color& color) = 0;
		virtual void Clear() = 0;

		virtual void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray, uint32_t indexCount = 0) = 0;
		virtual void DrawLines(const Ref<VertexArray>& vertexArray, uint32_t vertexCount) = 0;
		virtual void DrawQuad() = 0;

		virtual void SetLineWidth(float width) = 0;

		inline static API GetAPI() { return s_API; }
	private:
		static API s_API;
	};
}
