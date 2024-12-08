#pragma once

#include "Rhombus/Core/UUID.h"
#include "Rhombus/Math/Vector.h"

namespace rhombus
{
	class SpriteRendererComponent;
}

namespace ImGui
{
	enum RhombusLogType { LOG_DEBUG, LOG_INFO, LOG_WARNING, LOG_ERROR};

	bool InputUUID(const char* label, rhombus::UUID* v, int step = 1, int step_fast = 100, int flags = 0);
	bool IsItemReleased(int mouse_button = 0);
	bool SelectableEnum(const char* label, const char** enumStrings, int enumCount, int* currentEnumValue);
	void Sprite(const rhombus::SpriteRendererComponent& src, float height, bool center = true);
	void SpriteButton(const rhombus::SpriteRendererComponent& src, float size);
	void ShowRhombusDebugLogWindow();
	void RhombusDebugLog(const char* fmt, ...);
	void RhombusDebugLogInfo(const char* fmt, ...);
	void RhombusDebugLogWarning(const char* fmt, ...);
	void RhombusDebugLogError(const char* fmt, ...);
	void RhombusDebugLog(RhombusLogType logType, const char* fmt, va_list args);
}
