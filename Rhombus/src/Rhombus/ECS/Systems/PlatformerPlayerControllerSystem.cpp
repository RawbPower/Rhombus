#include "rbpch.h"
#include "PlatformerPlayerControllerSystem.h"
#include "Rhombus/ECS/Components/PlatformerPlayerControllerComponent.h"
#include "Rhombus/ECS/Components/PixelPlatformerBodyComponent.h"
#include "Rhombus/ECS/Components/TransformComponent.h"
#include "Rhombus/Core/Input.h"
#include "Rhombus/Core/KeyCodes.h"
#include "Rhombus/Core/GamepadCodes.h"

namespace rhombus
{
	void PlatformerPlayerControllerSystem::Update(DeltaTime dt)
	{
		float keyboardInput = float(Input::IsKeyPressed(RB_KEY_D) - Input::IsKeyPressed(RB_KEY_A));
		float gamepadInput = Input::GetGamepadAxis(0);
		float input = math::Abs(keyboardInput) > math::Abs(gamepadInput) ? keyboardInput : gamepadInput;

		for (Entity entity : GetEntities())
		{
			PlatformerPlayerControllerComponent& playerController = entity.GetComponent<PlatformerPlayerControllerComponent>();
			float speed = 0.0f;

			if (input != 0.0f)
			{
				speed = input * playerController.m_speed;
			}
			else
			{
				speed = 0.0f;
			}

			if (entity.HasComponent<PixelPlatformerBodyComponent>())
			{
				PixelPlatformerBodyComponent& physicsBody = entity.GetComponent<PixelPlatformerBodyComponent>();
				physicsBody.m_velocity.x = speed;

				if (!physicsBody.GetIsInAir())
				{
					playerController.ResetJumps();
				}

				// Jump code
				if (playerController.m_jumpBufferCount > 0.0f && playerController.m_coyoteTimeCount > 0.0f)
				{
					if (playerController.CanJump())
					{
						// Linear equation of motion v^2 = u^2 + 2as
						float jumpVelocity = playerController.m_maxJumpSpeed - GRAVITY * dt;
						physicsBody.m_velocity.y = jumpVelocity;

						playerController.AddJump();
						playerController.m_jumpBufferCount = 0.0f;
					}
				}

				if (playerController.m_cancelJump)
				{
					if (physicsBody.GetIsInAir() && playerController.m_currentJumpCount > 0 && physicsBody.m_velocity.y > playerController.m_minJumpSpeed)
					{
						physicsBody.m_velocity.y = playerController.m_minJumpSpeed;
					}

					playerController.m_cancelJump = false;
				}

				// Coyote timer
				if (physicsBody.GetIsInAir())
				{
					playerController.m_coyoteTimeCount -= dt;
				}
				else
				{
					playerController.m_coyoteTimeCount = playerController.m_coyoteTime;
				}
			}

			// Jump buffer
			if (playerController.m_jumpBufferCount >= 0)
			{
				playerController.m_jumpBufferCount -= dt;
			}
		}
	}

	void PlatformerPlayerControllerSystem::OnKeyPressed(int keycode, bool isRepeat)
	{
		bool shouldJump = !isRepeat && keycode == RB_KEY_SPACE;
		if (shouldJump)
		{
			ProcessJump();
		}
	}

	void PlatformerPlayerControllerSystem::OnKeyReleased(int keycode)
	{
		if (keycode == RB_KEY_SPACE)
		{
			CancelJump();
		}
	}

	void PlatformerPlayerControllerSystem::OnGamepadButtonDown(int button)
	{
		bool shouldJump = button == RB_GAMEPAD_BUTTON_A;
		if (shouldJump)
		{
			ProcessJump();
		}
	}

	void PlatformerPlayerControllerSystem::OnGamepadButtonUp(int button)
	{
		if (button == RB_GAMEPAD_BUTTON_A)
		{
			CancelJump();
		}
	}

	void PlatformerPlayerControllerSystem::ProcessJump()
	{
		for (Entity entity : GetEntities())
		{
			// Jump code
			PlatformerPlayerControllerComponent& playerController = entity.GetComponent<PlatformerPlayerControllerComponent>();
			playerController.m_jumpBufferCount = playerController.m_jumpBufferTime;
		}
	}

	void PlatformerPlayerControllerSystem::CancelJump()
	{
		for (Entity entity : GetEntities())
		{

			PlatformerPlayerControllerComponent& playerController = entity.GetComponent<PlatformerPlayerControllerComponent>();
			playerController.m_cancelJump = true;
		}
	}
}