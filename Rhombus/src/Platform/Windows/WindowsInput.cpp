#include "rbpch.h"
#include "WindowsInput.h"

// It's fine to this as we a inside a platform specific file
#include "Rhombus/Core/Application.h"
#include <SDL.h>

namespace rhombus 
{

	Input* Input::s_Instance = new WindowsInput();

	bool  WindowsInput::IsKeyPressedImpl(int keycode)
	{
		const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
						
		return currentKeyStates[SDL_GetScancodeFromKey((SDL_Keycode)keycode)];
	}

	bool WindowsInput::IsMouseButtonPressedImpl(int button)
	{
		int x, y;
		Uint32 b;

		SDL_PumpEvents();  // make sure we have the latest mouse state.
		b = SDL_GetMouseState(&x, &y);

		return (b & button) != 0;
	}

	std::pair<float, float> WindowsInput::GetMousePositionImpl()
	{
		int x, y;

		SDL_PumpEvents();  // make sure we have the latest mouse state.
		SDL_GetMouseState(&x, &y);

		return { (float)x, (float)y };
	}

	float WindowsInput::GetMouseXImpl()
	{
		auto[x, y] = GetMousePositionImpl();

		return x;
	}

	float WindowsInput::GetMouseYImpl()
	{
		auto[x, y] = GetMousePositionImpl();

		return y;
	}

}