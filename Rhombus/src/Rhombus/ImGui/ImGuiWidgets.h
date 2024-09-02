#pragma once

#include "Rhombus/Core/UUID.h"

namespace ImGui
{
	bool InputUUID(const char* label, rhombus::UUID* v, int step = 1, int step_fast = 100, int flags = 0);
	bool IsItemReleased(int mouse_button = 0);
	bool SelectableEnum(const char* label, const char** enumStrings, int enumCount, int* currentEnumValue);
}
