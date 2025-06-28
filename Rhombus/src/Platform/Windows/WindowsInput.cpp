#include "rbpch.h"
#include "Rhombus/Core/Input.h"

// It's fine to this as we a inside a platform specific file
#include "Rhombus/Core/Application.h"
#include <SDL.h>

namespace rhombus
{
	bool  Input::IsKeyPressed(int keycode)
	{
		const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);

		return currentKeyStates[SDL_GetScancodeFromKey((SDL_Keycode)keycode)];
	}

	bool Input::IsMouseButtonPressed(int button)
	{
		int x, y;
		Uint32 b;

		SDL_PumpEvents();  // make sure we have the latest mouse state.
		b = SDL_GetMouseState(&x, &y);

		return (b & SDL_BUTTON(button)) != 0;
	}

	Vec2 Input::GetMousePosition()
	{
		int x, y;

		SDL_PumpEvents();  // make sure we have the latest mouse state.
		SDL_GetMouseState(&x, &y);

		return { (float)x, (float)y };
	}

	float Input::GetMouseX()
	{
		auto [x, y] = GetMousePosition();

		return x;
	}

	float Input::GetMouseY()
	{
		auto [x, y] = GetMousePosition();

		return y;
	}

	bool Input::IsGamepadButtonPressed(int button)
	{
		SDL_Joystick* joystick = (SDL_Joystick*)Application::Get().GetWindow().GetJoystick();
		return joystick ? SDL_JoystickGetButton(joystick, button) != 0 : false;
	}

	float Input::GetGamepadAxis(int axis)
	{
		SDL_Joystick* joystick = (SDL_Joystick*)Application::Get().GetWindow().GetJoystick();
		if (!joystick)
		{
			return 0.0f;
		}

		int joyAxis = SDL_JoystickGetAxis(joystick, axis);
		if ((joyAxis > -3200) && (joyAxis < 3200))
		{
			return 0.0f;
		}

		return joyAxis > 0 ? joyAxis / 32767.0f : joyAxis / 32768.0f;
	}
}