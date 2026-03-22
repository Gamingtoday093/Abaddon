project "RVO2"
    kind "StaticLib"
    language "C++"
    cppdialect "C++20"
    
	targetdir ("bin/" .. TargetDir .. "/%{prj.name}")
    objdir ("bin-int/" .. ObjectDir .. "/%{prj.name}")
	
	files {
        "src/**.h",
        "src/**.cpp"
    }
	
	filter "system:windows"
        systemversion "latest"

    filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"
