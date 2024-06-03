project "Rhombus-ScriptRegistry"
	kind "SharedLib"
	language "C++"
	cppdialect "C++17"

	targetname "rhombus"
	targetdir ("%{wks.location}/bin/" .. outputdir .. "/Rhombus-Editor")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/Rhombus-Editor")

	files 
	{
		"src/**.h",
		"src/**.cpp"
	}
	
	includedirs
	{
		"%{wks.location}/Rhombus/src",
		"%{wks.location}/Rhombus/vendor",
		"%{wks.location}/Rhombus/vendor/spdlog/include",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}",
		"%{IncludeDir.SDL2}",
		"%{IncludeDir.Box2d}",
		"%{IncludeDir.Lua}"
	}

	links
	{
		"Rhombus"
	}
	
	filter "system:windows"
		systemversion "latest"

		defines
		{
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