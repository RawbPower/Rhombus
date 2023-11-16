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

	std::pair<float, float> Input::GetMousePosition()
	{
		int x, y;

		SDL_PumpEvents();  // make sure we have the latest mouse state.
		SDL_GetMouseState(&x, &y);

		return { (float)x, (float)y };
	}

	float Input::GetMouseX()
	{
		auto[x, y] = GetMousePosition();

		return x;
	}

	float Input::GetMouseY()
	{
		auto[x, y] = GetMousePosition();

		return y;
	}

}