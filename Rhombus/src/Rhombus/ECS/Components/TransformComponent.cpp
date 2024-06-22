#include "rbpch.h"
#include "TransformComponent.h"

#include "Rhombus/Math/Math.h"
#include "Rhombus/Math/Matrix.h"
#include "Rhombus/Math/Quat.h"

namespace rhombus
{
	Mat4 TransformComponent::GetTransform() const
	{
		Quat quat = Quat(m_rotation);
		Mat4 rotation = quat.ToMat4();

		Mat4 transform = math::Translate(Mat4::Identity(), m_position)
			* rotation
			* math::Scale(Mat4::Identity(), m_scale);

		return transform;
	}

	void TransformComponent::SetLayer(Z_LAYER layer)
	{
		m_position.z = zLayers[(int)layer];
	}

	void TransformComponent::SetPositionByLayerSection(Z_LAYER layer, int section, int numOfSections)
	{
		const float layerZ = zLayers[(int)layer];
		const float nextLayerZ = zLayers[(int)layer+1];
		const float layerRange = nextLayerZ - layerZ;

		const float posZ = layerZ + section * (layerRange / numOfSections);
		m_position.z = posZ;
	}
}