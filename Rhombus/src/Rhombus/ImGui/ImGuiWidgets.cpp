#include "rbpch.h"
#include "ImGuiWidgets.h"

#include "imgui.h"
#include "imgui_internal.h"

#include "Rhombus/Math/Vector.h"
#include "Rhombus/ECS/Components/SpriteRendererComponent.h"

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

	void Sprite(const rhombus::SpriteRendererComponent& src, float height, bool center)
	{
		ImVec2 uv0 = ImVec2{ 0, 1 };
		ImVec2 uv1 = ImVec2{ 1, 0 };
		float textureWidth = (float)src.m_texture->GetWidth();
		float textureHeight = (float)src.m_texture->GetHeight();
		if (src.UseSubTexture())
		{
			uv0 = ImVec2(src.m_subtexture->GetTexCoords()[3].x, src.m_subtexture->GetTexCoords()[3].y);
			uv1 = ImVec2(src.m_subtexture->GetTexCoords()[1].x, src.m_subtexture->GetTexCoords()[1].y);
			textureWidth = src.m_subtexture->GetWidth();
			textureHeight = src.m_subtexture->GetHeight();
		}

		const float fAspectRatio = textureWidth / textureHeight;
		const float spriteSize = height;
		const rhombus::Vec2 imageSize = rhombus::Vec2(spriteSize, spriteSize * (1.0f / fAspectRatio));

		if (center)
		{
			rhombus::Vec2 imageOffset = rhombus::Vec2((ImGui::GetWindowSize().x - imageSize.x) * 0.5f, (ImGui::GetWindowSize().y - imageSize.y) * 0.5f);
			ImGui::SetCursorPos({ imageOffset.x, imageOffset.y });		// Center image
		}

		uint32_t textureID = src.m_texture->GetRendererID();
		ImGui::Image((void*)textureID, ImVec2(imageSize.x, imageSize.y), uv0, uv1);
	}

	void SpriteButton(const rhombus::SpriteRendererComponent& src, float size)
	{
		ImVec2 uv0 = ImVec2{ 0, 1 };
		ImVec2 uv1 = ImVec2{ 1, 0 };
		float textureWidth = (float)src.m_texture->GetWidth();
		float textureHeight = (float)src.m_texture->GetHeight();
		if (src.UseSubTexture())
		{
			uv0 = ImVec2(src.m_subtexture->GetTexCoords()[3].x, src.m_subtexture->GetTexCoords()[3].y);
			uv1 = ImVec2(src.m_subtexture->GetTexCoords()[1].x, src.m_subtexture->GetTexCoords()[1].y);
			textureWidth = src.m_subtexture->GetWidth();
			textureHeight = src.m_subtexture->GetHeight();
		}

		// Caclulate image size
		const float fAspectRatio = textureWidth / textureHeight;
		const bool bWideImage = fAspectRatio >= 1.0f;
		const rhombus::Vec2 iconSize = bWideImage ? rhombus::Vec2(size, size * (1.0f / fAspectRatio)) : rhombus::Vec2(size * fAspectRatio, size);

		// Caclulate image padding needed to make square button
		const float fDimensionDifference = rhombus::math::Abs(textureWidth - textureHeight);
		const float fMaxDimension = bWideImage ? textureWidth : textureHeight;
		const float fSmallDimensionPadding = (fDimensionDifference / fMaxDimension) * size * 0.5f;
		const ImVec2 iconPadding = bWideImage ? ImVec2(0.0f, fSmallDimensionPadding) : ImVec2(fSmallDimensionPadding, 0.0f);

		uint32_t textureID = src.m_texture->GetRendererID();
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, iconPadding);
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0, 0, 0, 0));
		ImGui::ImageButton((void*)textureID, ImVec2(iconSize.x, iconSize.y), uv0, uv1);
		ImGui::PopStyleColor();
		ImGui::PopStyleColor();
		ImGui::PopStyleVar();
	}

	enum RhmobusDebugLogFlags
	{
		// Event types
		RhombusDebugLogFlags_None = 0,
		RhombusDebugLogFlags_Debug = 1 << 0,
		RhombusDebugLogFlags_Info = 1 << 1,
		RhombusDebugLogFlags_Warn = 1 << 2,
		RhombusDebugLogFlags_Error = 1 << 3,
		RhombusDebugLogFlags_All = RhombusDebugLogFlags_Debug | RhombusDebugLogFlags_Info | RhombusDebugLogFlags_Warn | RhombusDebugLogFlags_Error
	};

	static int debugLogFlags = RhombusDebugLogFlags_All;

	bool PushLogLineStyle(const char* begin)
	{
		if (strncmp(begin, "[INFO]", 6) == 0)
		{
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.75f, 1.0f, 0.75f, 1.0f));
			return true;
		}
		else if (strncmp(begin, "[WARNING]", 9) == 0)
		{
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 0.5f, 1.0f));
			return true;
		}
		else if (strncmp(begin, "[ERROR]", 7) == 0)
		{
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.5f, 0.25f, 1.0f));
			return true;
		}

		return false;
	}

	void ShowRhombusDebugLogWindow()
	{
		ImGuiContext& g = *GImGui;
		ImGui::Begin("Debug Log");

		ImGui::CheckboxFlags("All", &debugLogFlags, RhombusDebugLogFlags_All);
		ImGui::SameLine(); ImGui::CheckboxFlags("Debug", &debugLogFlags, RhombusDebugLogFlags_Debug);
		ImGui::SameLine(); ImGui::CheckboxFlags("Info", &debugLogFlags, RhombusDebugLogFlags_Info);
		ImGui::SameLine(); ImGui::CheckboxFlags("Warn", &debugLogFlags, RhombusDebugLogFlags_Warn);
		ImGui::SameLine(); ImGui::CheckboxFlags("Error", &debugLogFlags, RhombusDebugLogFlags_Error);

		if (ImGui::SmallButton("Clear"))
		{
			g.DebugLogBuf.clear();
			g.DebugLogIndex.clear();
		}
		ImGui::SameLine();
		if (ImGui::SmallButton("Copy"))
			ImGui::SetClipboardText(g.DebugLogBuf.c_str());
		ImGui::BeginChild("##log", ImVec2(0.0f, 0.0f), ImGuiChildFlags_Border, ImGuiWindowFlags_AlwaysVerticalScrollbar | ImGuiWindowFlags_AlwaysHorizontalScrollbar);

		ImGuiListClipper clipper;
		clipper.Begin(g.DebugLogIndex.size());
		int stepIndex = 0;
		while (clipper.Step())
		{
			for (int line_no = clipper.DisplayStart; line_no < clipper.DisplayEnd; line_no++)
			{
				const char* line_begin = g.DebugLogIndex.get_line_begin(g.DebugLogBuf.c_str(), line_no);
				const char* line_end = g.DebugLogIndex.get_line_end(g.DebugLogBuf.c_str(), line_no);

				bool needsStylePop = PushLogLineStyle(line_begin);

				ImGui::TextUnformatted(line_begin, line_end);

				if (needsStylePop)
				{
					ImGui::PopStyleColor();
				}
			}
			stepIndex++;
		}
		if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
			ImGui::SetScrollHereY(1.0f);
		ImGui::EndChild();

		ImGui::End();
	}

	void RhombusDebugLog(const char* fmt, ...)
	{
		if (debugLogFlags & RhombusDebugLogFlags_Debug)
		{
			va_list args;
			va_start(args, fmt);
			RhombusDebugLog(RhombusLogType::LOG_DEBUG, fmt, args);
			va_end(args);
		}
	}

	void RhombusDebugLogInfo(const char* fmt, ...)
	{
		if (debugLogFlags & RhombusDebugLogFlags_Info)
		{
			va_list args;
			va_start(args, fmt);
			RhombusDebugLog(RhombusLogType::LOG_INFO, fmt, args);
			va_end(args);
		}
	}

	void RhombusDebugLogWarning(const char* fmt, ...)
	{
		if (debugLogFlags & RhombusDebugLogFlags_Warn)
		{
			va_list args;
			va_start(args, fmt);
			RhombusDebugLog(RhombusLogType::LOG_WARNING, fmt, args);
			va_end(args);
		}
	}

	void RhombusDebugLogError(const char* fmt, ...)
	{
		if (debugLogFlags & RhombusDebugLogFlags_Error)
		{
			va_list args;
			va_start(args, fmt);
			RhombusDebugLog(RhombusLogType::LOG_ERROR, fmt, args);
			va_end(args);
		}
	}


	const char* GetLogTypeTag(RhombusLogType logType)
	{
		switch (logType)
		{
		case RhombusLogType::LOG_INFO:
			return "[INFO] ";
			break;
		case RhombusLogType::LOG_WARNING:
			return "[WARNING] ";
			break;
		case RhombusLogType::LOG_ERROR:
			return "[ERROR] ";
			break;
		default:
			return "";
			break;
		}
	}

	void RhombusDebugLog(RhombusLogType logType, const char* fmt, va_list args)
	{
		ImGuiContext& g = *GImGui;
		const int old_size = g.DebugLogBuf.size();
		g.DebugLogBuf.appendf(GetLogTypeTag(logType));
		g.DebugLogBuf.appendf("[%05d] ", g.FrameCount);
		g.DebugLogBuf.appendfv(fmt, args);
		g.DebugLogBuf.appendf("\n");
		g.DebugLogIndex.append(g.DebugLogBuf.c_str(), old_size, g.DebugLogBuf.size());
		/*if (g.DebugLogFlags & ImGuiDebugLogFlags_OutputToTTY)
		{
			IMGUI_DEBUG_PRINTF("%s", g.DebugLogBuf.begin() + old_size);
		}*/
	}
}