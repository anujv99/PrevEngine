workspace "PrevEngine"
    architecture "x64"

    configurations {
        "Debug",
        "Release",
        "Distribute"
    }

outputDir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/"

project "PrevEngine"
    location "PrevEngine"
    kind "StaticLib"
    language "C++"

    targetdir ("bin/" .. outputDir .. "%{prj.name}")
    objdir ("bin-int/" .. outputDir .. "%{prj.name}")

	pchheader "pch.h"
	pchsource "PrevEngine/src/pch.cpp"
	
	files {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/src/**.c"
    }
	
	files {
		"vendor/sources/**.c",
		"vendor/sources/**.cpp",
		"vendor/sources/**.h"
    }
	
    includedirs {
        "%{prj.name}/src",
		"vendor/includes"
    }

	filter "files:**.c"
		flags { "NoPCH" }
	
    filter "system:windows"
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"

        defines {
            "PV_PLATFORM_WINDOWS",
            "PV_BUILD_LIB",
			"PV_RENDERING_API_OPENGL",
			"PV_ENABLE_ASSERTS"
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
	pchsource "Sandbox/src/pch.cpp"
	
	files {
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
	}
	
	includedirs {
		"PrevEngine",
		"%{prj.name}/src"
    }
	
	links {
		"PrevEngine"
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