workspace "PrevEngine"
    architecture "x64"

    configurations {
        "Debug",
        "Release",
        "Distribute"
    }

outputDir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/"

IncludeDir = {}
IncludeDir["GLAD"] = "PrevEngine/vendor/GLAD/include"
IncludeDir["ImGui"] = "PrevEngine/vendor/ImGui"

include "PrevEngine/vendor/GLAD"
include "PrevEngine/vendor/ImGui"

project "PrevEngine"
    location "PrevEngine"
    kind "StaticLib"
    language "C++"

    targetdir ("bin/" .. outputDir .. "%{prj.name}")
    objdir ("bin-int/" .. outputDir .. "%{prj.name}")

	pchheader "pch.h"
	
	files {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }
	
    includedirs {
        "%{prj.name}/src",
		"%{IncludeDir.GLAD}",
		"%{IncludeDir.ImGui}"
    }
	
	links {
		"GLAD",
		"ImGui"
	}

	filter "system:linux"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines {
            "PV_PLATFORM_LINUX",
            "PV_BUILD_LIB",
			"PV_RENDERING_API_OPENGL", --Use PV_RENDERING_API_DIRECTX for directx 11
			"PV_ENABLE_ASSERTS"
		}
		
		libdirs { os.findlib("X11") }

		links {
			"X11"
		}

		removefiles {
			"%{prj.name}/src/platform/windows/**.cpp",
			"%{prj.name}/src/platform/windows/**.h",
			"%{prj.name}/src/platform/windows/**.c"
		}

    filter "system:windows"
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"

        defines {
            "PV_PLATFORM_WINDOWS",
            "PV_BUILD_LIB",
			"PV_RENDERING_API_OPENGL", --Use PV_RENDERING_API_DIRECTX for directx 11
			"PV_ENABLE_ASSERTS"
		}
		
		removefiles {
			"%{prj.name}/src/platform/linux/**.cpp",
			"%{prj.name}/src/platform/linux/**.h",
			"%{prj.name}/src/platform/linux/**.c"
		}

    filter "configurations:Debug"
        defines "PV_DEBUG"
        symbols "On"

    filter "configurations:Release"
	    defines "PV_RELEASE"
	    optimize "On"

	filter "configurations:Distribute"
		defines "PV_DIST"
		optimize "On"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	
	targetdir ("bin/" .. outputDir .. "%{prj.name}")
    objdir ("bin-int/" .. outputDir .. "%{prj.name}")
	
	pchheader "pch.h"
	
	files {
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
	}
	
	includedirs {
		"PrevEngine/src",
		"%{prj.name}/src"
    }
	
	links {
		"PrevEngine"
	}
	
	filter "system:linux"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines {
            "PV_PLATFORM_LINUX"
		}
		
		libdirs { os.findlib("X11") }

		links {
			"PrevEngine",
			"X11"
		}

	filter "system:windows"
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"

        defines {
            "PV_PLATFORM_WINDOWS"
        }
	
	filter "configurations:Debug"
		defines "PV_DEBGU"
		symbols "On"

	filter "configurations:Release"
		defines "PV_RELEASE"
		optimize "On"

	filter "configurations:Distribute"
		defines "PV_DIST"
		optimize "On"
