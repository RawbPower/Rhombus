#include "rbpch.h"
#include "ImGuiWidgets.h"

#include "imgui.h"

namespace ImGui
{
	bool InputUUID(const char* label, rhombus::UUID* v, int step, int step_fast, int flags)
	{
		// Hexadecimal input provided as a convenience but the flag name is awkward. Typically you'd use InputText() to parse your own data, if you want to handle prefixes.
		const char* format = ((ImGuiInputTextFlags)flags & ImGuiInputTextFlags_CharsHexadecimal) ? "%08X" : "%d";
		flags |= ImGuiInputTextFlags_ReadOnly;
		return InputScalar(label, ImGuiDataType_U64, (void*)v, (void*)(step > 0 ? &step : NULL), (void*)(step_fast > 0 ? &step_fast : NULL), format, (ImGuiInputTextFlags)flags);
	}

	bool ImGui::IsItemReleased(int mouse_button)
	{
		return IsMouseReleased((ImGuiMouseButton)mouse_button) && IsItemHovered(ImGuiHoveredFlags_None);
	}

	bool SelectableEnum(const char* label, const char** enumStrings, int enumCount, int* currentEnumValue)
	{
		bool valueChanged = false;
		const char* currentEnumString = enumStrings[*currentEnumValue];
		if (ImGui::BeginCombo(label, currentEnumString))
		{
			for (int i = 0; i < enumCount; i++)
			{
				bool isSelected = currentEnumString == enumStrings[i];
				if (ImGui::Selectable(enumStrings[i], isSelected))
				{
					currentEnumString = enumStrings[i];
					*currentEnumValue = i;
					valueChanged = true;
				}

				if (isSelected)
					ImGui::SetItemDefaultFocus();
			}

			ImGui::EndCombo();
		}

		return valueChanged;
	}
}