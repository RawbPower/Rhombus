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
	public:
		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;

		Mat4 GetTransform() const;
		Mat4 GetWorldTransform() const;

		void SetLayer(Z_LAYER layer);
		void SetPositionByLayerSection(Z_LAYER layer, int section, int numOfSections);

		Vec3 m_position = { 0.0f, 0.0f, 0.0f };
		Vec3 m_rotation = { 0.0f, 0.0f, 0.0f };
		Vec3 m_scale = { 1.0f, 1.0f, 1.0f };

		Ref<SceneGraphNode> m_sceneGraphNode;
	};
}

