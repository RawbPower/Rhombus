include "./vendor/premake/premake_customization/solution_items.lua"

workspace "Rhombus"
	architecture "x86_64"
	startproject "Rhombus-Editor"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}
	
	solution_items
	{
		".editorconfig"
	}
	
	flags
	{
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["Glad"] = "%{wks.location}/Rhombus/vendor/Glad/include"
IncludeDir["ImGui"] = "%{wks.location}/Rhombus/vendor/imgui"
IncludeDir["glm"] = "%{wks.location}/Rhombus/vendor/glm"
IncludeDir["stb_image"] = "%{wks.location}/Rhombus/vendor/stb_image"
IncludeDir["SDL2"] = "%{wks.location}/Rhombus/vendor/SDL2/include"
IncludeDir["yaml_cpp"] = "%{wks.location}/Rhombus/vendor/yaml-cpp/include"
IncludeDir["ImGuizmo"] = "%{wks.location}/Rhombus/vendor/ImGuizmo"
IncludeDir["Box2d"] = "%{wks.location}/Rhombus/vendor/box2d/include"
IncludeDir["Lua"] = "%{wks.location}/Rhombus/vendor/Lua/include"

group "Dependencies"
	include "Rhombus/vendor/Glad"
	include "Rhombus/vendor/imgui"
	include "Rhombus/vendor/yaml-cpp"
	include "Rhombus/vendor/box2d"

group ""

include "Rhombus"
include "Rhombus-Editor"
include "Volley"
include "Patience"
include "Rhombus-ScriptRegistry"