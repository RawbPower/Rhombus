#pragma once

#include "Component.h"

namespace rhombus
{
	class PlatformerPlayerControllerComponent : public ComponentBase
	{
	public:
		friend class PlatformerPlayerControllerSystem;

		PlatformerPlayerControllerComponent() = default;
		PlatformerPlayerControllerComponent(const PlatformerPlayerControllerComponent& other) = default;

		virtual void OnComponentAdded() override;

		float m_speed = 0.0f;
		float m_minJumpHeight = 0.0f;
		float m_maxJumpHeight = 0.0f;
		float m_doubleJumpHeight = 0.0f;
		float m_jumpBufferTime = 0.1f;
		float m_coyoteTime = 0.1f;

		void AddJump() { m_currentJumpCount++; }
		void ResetJumps() { m_currentJumpCount = 0; }
		bool CanJump() const { return m_currentJumpCount < 1; }
	private:
		bool m_cancelJump = false;
		float m_minJumpSpeed = 0.0f;
		float m_maxJumpSpeed = 0.0f;
		float m_jumpBufferCount = 0.0f;
		float m_coyoteTimeCount = 0.0f;
		int m_currentJumpCount = 0;
	};
}
