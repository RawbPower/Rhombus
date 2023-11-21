#pragma once

#include <glm/glm.hpp>

namespace rhombus
{
	class TagComponent
	{
	public:
		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string& tag) : m_tag(tag) {}

		std::string m_tag;
	private:
		friend class Scene;
	};

	class TransformComponent
	{
	public:
		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::mat4 & transform) : m_transform(transform) {}

		glm::mat4& GetTransform() { return m_transform; }

	private:
		glm::mat4 m_transform{ 1.0f };
	};

	class SpriteRendererComponent
	{
	public:
		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent&) = default;
		SpriteRendererComponent(const glm::vec4& color) : m_color(color) {}

		glm::vec4& GetColor() { return m_color; }

	private:
		glm::vec4 m_color{ 1.0f, 1.0f, 1.0f, 1.0f };
	};
}
