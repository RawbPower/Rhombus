#pragma once

#include "OrthographicCamera.h"
#include "Texture.h"

namespace rhombus
{
	class Renderer2D
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const OrthographicCamera& camera);
		static void EndScene();

		// Primitives
		static void DrawQuad(const glm::vec2& position, const float& angle, const glm::vec2& scale, const glm::vec4& color);
		static void DrawQuad(const glm::vec3& position, const float& angle, const glm::vec2& scale, const glm::vec4& color);
		static void DrawQuad(const glm::vec2& position, const float& angle, const glm::vec2& scale, const Ref<Texture2D>& texture);
		static void DrawQuad(const glm::vec2& position, const float& angle, const glm::vec2& scale, const Ref<Texture2D>& texture, const glm::vec4& color);
		static void DrawQuad(const glm::vec3& position, const float& angle, const glm::vec2& scale, const Ref<Texture2D>& texture);
		static void DrawQuad(const glm::vec3& position, const float& angle, const glm::vec2& scale, const Ref<Texture2D>& texture, const glm::vec4& color);

	private:
	};
}
