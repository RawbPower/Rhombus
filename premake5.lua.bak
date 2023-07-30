workspace "Rhombus"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

	flags
	{
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

startproject "Sandbox"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["Glad"] = "Rhombus/vendor/Glad/include"
IncludeDir["ImGui"] = "Rhombus/vendor/imgui"
IncludeDir["glm"] = "Rhombus/vendor/glm"
IncludeDir["stb_image"] = "Rhombus/vendor/stb_image"
IncludeDir["SDL2"] = "Rhombus/vendor/SDL2/include"

group "Dependencies"
	include "Rhombus/vendor/Glad"
	include "Rhombus/vendor/imgui"

group ""

project "Rhombus"
	location "Rhombus"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "rbpch.h"
	pchsource "Rhombus/src/rbpch.cpp"

	files 
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/stb_image/**.h",
		"%{prj.name}/vendor/stb_image/**.cpp",
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl"
	}

	defines 
	{
		"_CRT_SECURE_NO_WARNINGS"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}",
		"%{IncludeDir.SDL2}",
		"%{prj.name}/vendor/assimp/include"
	}
	
	libdirs
	{
		"Rhombus/vendor/SDL2/lib/x64"
	}

	links
	{
		"Glad",
		"ImGui",
		"opengl32.lib",
		"SDL2",
		"SDL2main"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"RB_BUILD_DLL",
		}

	filter "configurations:Debug"
		defines "RB_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "RB_Release"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "RB_DIST"
		runtime "Release"
		optimize "on"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files 
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"Rhombus/vendor/spdlog/include",
		"Rhombus/src",
		"Rhombus/vendor",
		"%{IncludeDir.glm}",
		"Rhombus/vendor/assimp/include",
	}

	links
	{
		"Rhombus",
		"Rhombus/vendor/assimp/lib/assimp.lib"
	}

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		defines "RB_DEBUG"
		runtime "Debug"
		symbols "on"


	filter "configurations:Release"
		defines "RB_Release"
		runtime "Release"
		optimize "on"


	filter "configurations:Dist"
		defines "RB_DIST"
		runtime "Release"
		optimize "on"

