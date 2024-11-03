project "Patience"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	files 
	{
		"src/**.h",
		"src/**.cpp"
	}

	includedirs
	{
		"%{wks.location}/Patience/src",
		"%{wks.location}/Rhombus/vendor/spdlog/include",
		"%{wks.location}/Rhombus/src",
		"%{wks.location}/Rhombus/vendor",
		"%{wks.location}/Rhombus-Editor/src",
		"%{IncludeDir.yaml_cpp}"
	}
	
	libdirs
	{
		"%{wks.location}/bin/" .. outputdir .. "/Rhombus-Editor"
	}

	links
	{
		"Rhombus",
		"Rhombus-Editor",
		"yaml-cpp"
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