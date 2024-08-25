#include "rbpch.h"
#include "ImGuiWidgets.h"

#include "imgui.h"

namespace ImGui
{
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