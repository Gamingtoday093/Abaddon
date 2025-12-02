project "ImGuizmo"
    kind "StaticLib"
    language "C++"
    cppdialect "C++20"
    staticruntime "on"
    
	targetdir ("bin/" .. TargetDir .. "/%{prj.name}")
    objdir ("bin-int/" .. ObjectDir .. "/%{prj.name}")

	files {
        "ImGuizmo.h",
        "ImGuizmo.cpp"
    }
	
	includedirs {
        "%{wks.location}/vendor/external/ImGui"
    }
	
	filter "system:windows"
        systemversion "latest"

    filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"
