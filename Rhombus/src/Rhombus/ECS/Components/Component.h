#pragma once

#include "Rhombus/Core/UUID.h"
#include "Rhombus/Math/Math.h"
#include "Rhombus/Math/Matrix.h"
#include "Rhombus/Math/Quat.h"

#include "Rhombus/ECS/Entity.h"

namespace rhombus
{
	class ComponentBase
	{
	public:
		virtual void OnComponentAdded() {}

		void SetOwnerEntity(Entity entity) { m_ownerEntity = entity; }
		Entity GetOwnerEntity() const { return m_ownerEntity; }

	private:
		Entity m_ownerEntity;
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

		Mat4 GetTransform() const
		{
			Quat quat = Quat(m_rotation);
			Mat4 rotation = quat.ToMat4();

			Mat4 transform = math::Translate(Mat4::Identity(), m_position)
				* rotation
				* math::Scale(Mat4::Identity(), m_scale);

			return transform;
		}

		Vec3 m_position = { 0.0f, 0.0f, 0.0f };
		Vec3 m_rotation = { 0.0f, 0.0f, 0.0f };
		Vec3 m_scale = { 1.0f, 1.0f, 1.0f };
	};
}
