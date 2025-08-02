#pragma once

#include "Component.h"

namespace rhombus
{
	class PlatformerPlayerControllerComponent : public ComponentBase
	{
	public:
		PlatformerPlayerControllerComponent() = default;
		PlatformerPlayerControllerComponent(const PlatformerPlayerControllerComponent& other) = default;

		float m_speed = 0.0f;
		float m_jumpHeight = 0.0f;
		float m_doubleJumpHeight = 0.0f;

		bool GetShouldJump() const { return m_shouldJump; }
		void SetShouldJump(bool jump) { m_shouldJump = jump; }

		void AddJump() { m_currentJumpCount++; }
		void ResetJumps() { m_currentJumpCount = 0; }
		bool CanJump() const { return m_currentJumpCount < 2; }
	private:
		bool m_shouldJump = false;
		int m_currentJumpCount = 0;
	};
}
