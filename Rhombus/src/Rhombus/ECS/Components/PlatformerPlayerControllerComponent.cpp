#include "rbpch.h"
#include "PlatformerPlayerControllerComponent.h"

namespace rhombus
{
	void PlatformerPlayerControllerComponent::OnComponentAdded()
	{
		m_maxJumpSpeed = math::Sqrt(-2 * GRAVITY * m_maxJumpHeight);
		m_minJumpSpeed = math::Sqrt(-2 * GRAVITY * m_minJumpHeight);
	}
}