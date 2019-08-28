project "Game"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir "bin/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
	objdir  "build/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

	files {
		"src/Game/Game.cpp",
		"src/Came/Game.h"
	}

	includedirs {
		"../Engine/src",
		"../ThirdParty/glad/include",
		"../ThirdParty/glfw/include"
	}