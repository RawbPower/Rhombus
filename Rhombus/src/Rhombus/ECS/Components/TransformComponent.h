#pragma once

#include "Component.h"

#include "Rhombus/Math/Vector.h"
#include <map>

namespace rhombus
{
	enum Z_LAYER { BACKGROUND_LAYER = 0, MIDDLEGROUND_LAYER, FOREGROUND_1_LAYER, FOREGROUND_2_LAYER, FOREGROUND_3_LAYER, UI_LAYER, LAYER_COUNT };

	const float zLayers[] = {-0.5f, -0.25f, 0.0f, 0.25f, 0.5f, 0.75f, 1.0f};

	class TransformComponent : public ComponentBase
	{
	private:
		Vec3 m_position = { 0.0f, 0.0f, 0.0f };
		Vec3 m_rotation = { 0.0f, 0.0f, 0.0f };
		Vec3 m_scale = { 1.0f, 1.0f, 1.0f };

	public:
		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;

		Vec3 GetPosition() const { return m_position; }
		Vec3 GetRotation() const { return m_rotation; }
		Vec3 GetScale() const { return m_scale; }
		Mat4 GetTransform() const;

		Vec3& GetPositionRef();
		Vec3& GetRotationRef();
		Vec3& GetScaleRef();

		Vec3 GetWorldPosition() const;
		Vec3 GetWorldRotation() const;
		Vec3 GetWorldScale() const;
		Mat4 GetWorldTransform() const;

		void SetPosition(Vec2 position);
		void SetPosition(Vec3 position);
		void SetRotation(float rotation);
		void SetRotation(Vec3 rotation);
		void SetScale(Vec3 scale);
		void SetTransform(Mat4 transform);

		void SetWorldTransform(Mat4 transform);
		void SetWorldPosition(Vec3 position);

		void SetLayer(Z_LAYER layer);
		void SetPositionByLayerSection(Z_LAYER layer, int section, int numOfSections);

		Ref<SceneGraphNode> m_sceneGraphNode;
	};
}

