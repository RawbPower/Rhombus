#include "PlayerInputSystem.h"

void PlayerInputSystem::Update()
{
	float keyboardInput = float(Input::IsKeyPressed(RB_KEY_D) - Input::IsKeyPressed(RB_KEY_A));
	float gamepadInput = Input::GetGamepadAxis(0);
	float input = math::Abs(keyboardInput) > math::Abs(gamepadInput) ? keyboardInput : gamepadInput;

	for (Entity entity : GetEntities())
	{
		AnimatorComponent& animator = entity.GetComponent<AnimatorComponent>();

		const PixelPlatformerBodyComponent& body = entity.GetComponentRead<PixelPlatformerBodyComponent>();

		if (body.GetIsInAir())
		{
			if (body.m_velocity.y < 0.0f)
			{
				if (!animator.IsPlaying("Jump Down"))
				{
					animator.Play("Jump Down");
				}
			}
			else
			{
				if (!animator.IsPlaying("Jump Up"))
				{
					animator.Play("Jump Up");
				}
			}
		}
		else
		{
			const bool bHasRunInput = input != 0.0f;

			if (bHasRunInput && !animator.IsPlaying("Run"))
			{
				animator.Play("Run");
			}
			else if (!bHasRunInput && !animator.IsPlaying("Idle"))
			{
				animator.Play("Idle");
			}
		}

		SpriteRendererComponent& sprite = entity.GetComponent<SpriteRendererComponent>();
		if (input > 0.0f)
		{
			sprite.SetFlipX(false);
		}
		else if (input < 0.0f)
		{
			sprite.SetFlipX(true);
		}
	}
}