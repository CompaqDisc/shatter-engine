workspace "Shatter-Engine"
	architecture "x86_64"
	configurations { "Debug", "Release", "Development" }

project "Engine"
	kind "StaticLib"
	cppdialect "C++14"

	filter "configurations:Debug"
		defines { "DEBUG" }
		symbols "On"

	filter "configurations:Release"
		defines { "NDEBUG" }
		optimize "On"


project "Game"
	kind "ConsoleApp"
	cppdialect "C++14"