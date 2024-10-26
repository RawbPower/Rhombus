#include "rbpch.h"
#include "TransformComponent.h"

#include "Rhombus/Scenes/SceneGraphNode.h"

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

	Vec3& TransformComponent::GetPositionRef()
	{
		m_sceneGraphNode->SetIsDirty(true); 
		return m_position; 
	}

	Vec3& TransformComponent::GetRotationRef()
	{
		m_sceneGraphNode->SetIsDirty(true);
		return m_rotation;
	}

	Vec3& TransformComponent::GetScaleRef()
	{
		m_sceneGraphNode->SetIsDirty(true);
		return m_scale;
	}

	Vec3 TransformComponent::GetWorldPosition() const
	{ 
		Mat4 worldTransform = GetWorldTransform();
		Vec3 position, rotation, scale;
		math::DecomposeTransform(worldTransform, position, rotation, scale);
		return position; 
	}

	Vec3 TransformComponent::GetWorldRotation() const
	{ 
		Mat4 worldTransform = GetWorldTransform();
		Vec3 position, rotation, scale;
		math::DecomposeTransform(worldTransform, position, rotation, scale);
		return rotation; 
	}

	Vec3 TransformComponent::GetWorldScale() const
	{ 
		Mat4 worldTransform = GetWorldTransform();
		Vec3 position, rotation, scale;
		math::DecomposeTransform(worldTransform, position, rotation, scale);
		return scale; 
	}

	Mat4 TransformComponent::GetWorldTransform() const
	{
		return m_sceneGraphNode->GetWorldTransform();
	}

	void TransformComponent::SetPosition(Vec2 position)
	{
		m_position.x = position.x;
		m_position.y = position.y;
		m_sceneGraphNode->SetIsDirty(true);
	}

	void TransformComponent::SetPosition(Vec3 position)
	{
		m_position = position;
		m_sceneGraphNode->SetIsDirty(true);
	}

	void TransformComponent::SetRotation(float rotation)
	{
		m_rotation.z = rotation;
		m_sceneGraphNode->SetIsDirty(true);
	}

	void TransformComponent::SetRotation(Vec3 rotation)
	{
		m_rotation = rotation;
		m_sceneGraphNode->SetIsDirty(true);
	}

	void TransformComponent::SetScale(Vec3 scale)
	{
		m_scale = scale;
		m_sceneGraphNode->SetIsDirty(true);
	}

	void TransformComponent::SetTransform(Mat4 transform)
	{
		Vec3 position, rotation, scale;
		math::DecomposeTransform(transform, position, rotation, scale);
		m_position = position;
		m_rotation = rotation;				// TODO: Look into gimbal lock issue
		m_scale = scale;
		m_sceneGraphNode->SetIsDirty(true);
	}

	void TransformComponent::SetWorldTransform(Mat4 transform)
	{
		Mat4 localTransform;
		if (m_sceneGraphNode->GetParent())
		{
			localTransform = m_sceneGraphNode->GetParent()->GetWorldTransform().Inverse() * transform;
		}
		else
		{
			localTransform = transform;
		}

		SetTransform(localTransform);

		m_sceneGraphNode->SetIsDirty(true);
	}

	void TransformComponent::SetWorldPosition(Vec3 position)
	{
		if (m_sceneGraphNode->GetParent())
		{
			Mat4 newWorldMatrix = m_sceneGraphNode->GetWorldTransform();
			newWorldMatrix.SetD(position);
			Mat4 localTransform = m_sceneGraphNode->GetParent()->GetWorldTransform().Inverse() * newWorldMatrix;
			m_position = localTransform.d();
		}
		else
		{
			m_position = position;
		}

		m_sceneGraphNode->SetIsDirty(true);
	}

	void TransformComponent::SetLayer(Z_LAYER layer)
	{
		Vec3 worldPos = GetWorldPosition();
		SetWorldPosition(Vec3(worldPos.x, worldPos.y, zLayers[(int)layer]));
	}

	void TransformComponent::SetPositionByLayerSection(Z_LAYER layer, int section, int numOfSections)
	{
		const float layerZ = zLayers[(int)layer];
		const float nextLayerZ = zLayers[(int)layer+1];
		const float layerRange = nextLayerZ - layerZ;

		const float posZ = layerZ + section * (layerRange / numOfSections);

		Vec3 worldPos = GetWorldPosition();
		SetWorldPosition(Vec3(worldPos.x, worldPos.y, posZ));
	}
}