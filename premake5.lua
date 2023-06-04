workspace "Game-Engine"
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
IncludeDir["GLFW"] = "Game-Engine/vendor/GLFW/include"
IncludeDir["Glad"] = "Game-Engine/vendor/Glad/include"
IncludeDir["ImGui"] = "Game-Engine/vendor/imgui"
IncludeDir["glm"] = "Game-Engine/vendor/glm"
IncludeDir["stb_image"] = "Game-Engine/vendor/stb_image"
IncludeDir["SDL2"] = "Game-Engine/vendor/SDL2/include"

group "Dependencies"
	include "Game-Engine/vendor/GLFW"
	include "Game-Engine/vendor/Glad"
	include "Game-Engine/vendor/imgui"

group ""

project "Game-Engine"
	location "Game-Engine"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "rbpch.h"
	pchsource "Game-Engine/src/rbpch.cpp"

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
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}",
		"%{IncludeDir.SDL2}",
		"%{prj.name}/vendor/assimp/include"
	}
	
	libdirs
	{
		"Game-Engine/vendor/SDL2/lib/x64"
	}

	links
	{
		"GLFW",
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
			"RB_PLATFORM_WINDOWS",
			"RB_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
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
		"Game-Engine/vendor/spdlog/include",
		"Game-Engine/src",
		"Game-Engine/vendor",
		"%{IncludeDir.glm}",
		"Game-Engine/vendor/assimp/include",
	}

	links
	{
		"Game-Engine",
		"Game-Engine/vendor/assimp/lib/assimp.lib"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"RB_PLATFORM_WINDOWS"
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

