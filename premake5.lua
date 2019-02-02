GCC_PATH = "" --provide gcc path if not added ni path

local gcc = premake.tools.gcc
gcc.tools = {
	cc = GCC_PATH .. "gcc",
	cxx = GCC_PATH .."g++",
	ar = GCC_PATH .. "ar"
}

function gcc.gettoolname(cfg, tool)
	return gcc.tools[tool]
end

--Finish Setting up gcc

workspace "PrevEngine"
	startproject "Sandbox"
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
IncludeDir["glm"] = "PrevEngine/vendor/glm/glm"

include "PrevEngine/vendor/GLAD"
include "PrevEngine/vendor/ImGui"
include "PrevEngine/vendor/glm"

--[[
Rendering API supported	 | renderingAPI
DirectX 				 | PV_RENDERING_API_DIRECTX
OpenGL					 | PV_RENDERING_API_OPENGL
]]--
renderingAPI = "PV_RENDERING_API_OPENGL"

--[[
Platform supported		| platform 
Window					| PV_PLATFORM_WINDOWS
linux					| PV_PLATFORM_LINUX
]]--
platform = "PV_PLATFORM_WINDOWS"

project "PrevEngine"
    location "PrevEngine"
    kind "StaticLib"
    language "C++"

    targetdir ("bin/" .. outputDir .. "%{prj.name}")
    objdir ("bin-int/" .. outputDir .. "%{prj.name}")

	files {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }
	
    includedirs {
        "%{prj.name}/src",
		"%{IncludeDir.GLAD}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}"
    }
	
	links {
		"GLAD",
		"ImGui"
	}

	filter "system:linux"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"
		
		if (renderingAPI ~= "PV_RENDERING_API_OPENGL" and platform == "PV_PLATFORM_LINUX") then
			error("Can's use directx on linux")
		end
		
		defines {
            platform,
            "PV_BUILD_LIB",
			renderingAPI,
			"PV_ENABLE_ASSERTS"
		}
		
		libdirs { os.findlib("X11") }

		links {
			"X11"
		}

		removefiles {
			"%{prj.name}/src/platform/windows/*",
			"%{prj.name}/src/api/DirectX/*"
		}
		
		pchheader "pch.h"

    filter "system:windows"
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"

        defines {
            platform,
			renderingAPI,
            "PV_BUILD_LIB",
			"PV_ENABLE_ASSERTS",
			"_CRT_SECURE_NO_WARNINGS"
		}
		
		removefiles {
			"%{prj.name}/src/platform/linux/*",
		}
		
		pchheader "pch.h"
		
	filter "action:vs*"
		pchsource "PrevEngine/src/pch.cpp"
		
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
	
	files {
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
	}
	
	includedirs {
		"PrevEngine/src",
		"%{prj.name}/src",
		"%{IncludeDir.glm}"
    }
	
	links {
		"PrevEngine"
	}
	
	filter "system:linux"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines {
            platform
		}
		
		libdirs { os.findlib("X11") }

		links {
			"PrevEngine",
			"X11"
		}
		
		pchheader "pch.h"

	filter "system:windows"
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"

        defines {
            platform,
			"_CRT_SECURE_NO_WARNINGS"
        }
		
		pchheader "pch.h"
		
	filter "action:vs*"
		pchsource "Sandbox/src/pch.cpp"
	
	filter "configurations:Debug"
		defines "PV_DEBGU"
		symbols "On"

	filter "configurations:Release"
		defines "PV_RELEASE"
		optimize "On"

	filter "configurations:Distribute"
		defines "PV_DIST"
		optimize "On"
