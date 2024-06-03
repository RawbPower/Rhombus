#pragma once

#include "Rhombus/Core/UUID.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace rhombus
{
	class ComponentBase
	{
		
	};

	class IDComponent : public ComponentBase
	{
	public:
		UUID m_id;

		IDComponent() = default;
		IDComponent(const IDComponent&) = default;
		IDComponent(const UUID id) : m_id(id) {}
	};

	class TagComponent : public ComponentBase
	{
	public:
		TagComponent() = default;
		TagComponent(const TagComponent&) = default;

		std::string m_tag;
	private:
		friend class Scene;
	};

	class TransformComponent : public ComponentBase
	{
	public:
		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;

		glm::mat4 GetTransform() const
		{
			/*glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), m_rotation.x, {1, 0, 0})
				* glm::rotate(glm::mat4(1.0f), m_rotation.y, { 0, 1, 0 })
				* glm::rotate(glm::mat4(1.0f), m_rotation.z, { 0, 0, 1 });*/

			glm::mat4 rotation = glm::toMat4(glm::quat(m_rotation));

			glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_position)
				* rotation
				* glm::scale(glm::mat4(1.0f), m_scale);

			return transform;
		}

		glm::vec3 m_position = { 0.0f, 0.0f, 0.0f };
		glm::vec3 m_rotation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 m_scale = { 1.0f, 1.0f, 1.0f };
	};
}
