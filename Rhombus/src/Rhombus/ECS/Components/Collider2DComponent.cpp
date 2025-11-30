#include "rbpch.h"
#include "Collider2DComponent.h"

#include "Rhombus.h"

namespace rhombus
{
	REGISTER_TYPE(BoxCollider2DComponent);
	REGISTER_MEMBER(BoxCollider2DComponent, m_offset);
	REGISTER_MEMBER(BoxCollider2DComponent, m_size);
	REGISTER_MEMBER(BoxCollider2DComponent, m_density);
	REGISTER_MEMBER(BoxCollider2DComponent, m_friction);
	REGISTER_MEMBER(BoxCollider2DComponent, m_restitution);
	REGISTER_MEMBER(BoxCollider2DComponent, m_restitutionThreshold);

	BoxCollider2DComponent::BoxCollider2DComponent()
	{
	}
}