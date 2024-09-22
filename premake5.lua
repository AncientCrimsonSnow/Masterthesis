workspace "Gengine"
	architecture "x64"
	startproject "Sandbox"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "Gengine/vendor/GLFW/include"
IncludeDir["Glad"] = "Gengine/vendor/Glad/include"
IncludeDir["ImGui"] = "Gengine/vendor/imgui"
IncludeDir["glm"] = "Gengine/vendor/glm"
IncludeDir["json"] = "Gengine/vendor/json/include"

group "Dependencies"
	include "Gengine/vendor/GLFW"
	include "Gengine/vendor/Glad"
	include "Gengine/vendor/imgui"
group ""

project "Gengine"
	location "Gengine"
	kind "StaticLib"
	language "C++"
	cppdialect "C++20"
	staticruntime "on"
	
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "ggpch.h"
	pchsource "Gengine/src/ggpch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl",
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
		"%{IncludeDir.json}"
	}

	links
	{
		"GLFW",
		"Glad",
		"ImGui",
		"opengl32.lib",
		"dwmapi.lib"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"GG_PLATFORM_WINDOWS",
			"GLFW_INCLUDE_NONE"
		}

	filter "configurations:Debug"
		defines "GG_DEBUG"
		runtime "Debug"
		symbols "on"
		
	filter "configurations:Release"
		defines "GG_RELEASE"
		runtime "Release"
		optimize "on"
		
	filter "configurations:Dist"
		defines "GG_DIST"
		runtime "Release"
		optimize "on"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"
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
		"Gengine/vendor/spdlog/include",
		"Gengine/src",
		"Gengine/vendor",
		"%{IncludeDir.glm}",
		"%{IncludeDir.json}",
		"Sandbox/src"
	}

	links
	{
		"Gengine"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"GG_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "GG_DEBUG"
		runtime "Debug"
		symbols "on"
		
	filter "configurations:Release"
		defines "GG_RELEASE"
		runtime "Release"
		optimize "on"
		
	filter "configurations:Dist"
		defines "GG_DIST"
		runtime "Release"
		optimize "on"