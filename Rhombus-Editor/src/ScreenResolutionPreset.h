#pragma once

#include "Rhombus/Core/Core.h"

namespace rhombus
{
	struct ScreenResolutionPreset
	{
		const char* name;
		uint32_t width;
		uint32_t height;
	};

	const ScreenResolutionPreset g_ScreenResolutionPresets[] = 
	{ 
		{"320x180", 320, 180},
		{"480x270", 480, 270},
		{"640x360", 640, 360},
		{"960x540", 960, 540},
		{"1920x1080", 1920, 1080},
		{"160x144 (Gameboy)", 160, 144},
		{"240x160 (GBA)", 240, 160},
		{"256x240 (NES)", 256, 240},
		{"256x224 (SNES)", 256, 224},
		{"160x192 (Atari)", 160, 192},
		{"256x192 (DS)", 256, 192},
		{"128x128 (Pico8)", 128, 128},
	};
}
