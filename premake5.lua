workspace "Shatter-Engine"
	architecture "x86_64"
	configurations { "Debug", "Release", "Development" }
	flags { "MultiProcessorCompile" }

group "Dependencies"
	include "Game"
	include "ThirdParty/glfw"
	include "ThirdParty/glad"
	include "ThirdParty/imgui-1.70"
	--include "ThirdParty/SFML-2.5.1"
group ""

--[[
	premake project format:
		kind
		language
		
		targetdir
		objdir

		files {}
		includedirs {}
		defines {}
		links {}

		filters...
]]

project "Engine"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir "bin/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/%{prj.name}"
	objdir  "build/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/%{prj.name}"

	files {
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/src/**.h"
	}

	includedirs {
		"%{prj.name}/src/",
		"Game/src/",
		"ThirdParty/discord-rpc/include",
		"ThirdParty/glad/include",
		"ThirdParty/glfw/include",
		"ThirdParty/glm",
		"ThirdParty/imgui-1.70",
		--"ThirdParty/SFML-2.5.1/include",
		"ThirdParty/stb"
	}

	defines {
		"_CRT_SECURE_NO_WARNINGS"
	}

	links {
		"glfw",
		"glad",
		"imgui"
	}

	filter "configurations:Debug"
		defines { "DEBUG" }
		symbols "On"

	filter "configurations:Release"
		defines { "NDEBUG" }
		optimize "On"

	filter "configurations:Development"
		defines { "DEVEL" }
		symbols "On"
		optimize "On"

	filter "system:windows"
		defines { "_WIN32", "AC_PLATFORM_WINDOWS" }

	filter "system:linux"
		defines { "AC_PLATFORM_LINUX" }


project "SampleProject"
	kind "WindowedApp"
	language "C++"
	cppdialect "C++17"

	targetdir "bin/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/%{prj.name}"
	objdir  "build/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/%{prj.name}"

	files {
		"%{prj.name}/SampleProject.cpp",
		"%{prj.name}/SampleProject.h"
	}
	includedirs {
		"Game/src",
		"Engine/src",
		"ThirdParty/discord-rpc/include",
		"ThirdParty/glad/include",
		"ThirdParty/glfw/include",
		"ThirdParty/glm",
		"ThirdParty/imgui-1.70",
		--"ThirdParty/SFML-2.5.1/include",
		"ThirdParty/stb"
	}

	links {
		"Engine",
		"Game",
		"imgui",
		"glad",
		"glfw",
		--"sfml",
		"dl",
		"X11",
		"pthread",
		"openal",
		"sfml-graphics",
		"sfml-window",
		"sfml-system",
		"sfml-audio"
	}