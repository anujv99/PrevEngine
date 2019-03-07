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
IncludeDir["GLAD"] 	= "PrevEngine/vendor/GLAD/include"
IncludeDir["ImGui"] = "PrevEngine/vendor/ImGui"
IncludeDir["glm"] 	= "PrevEngine/vendor/glm/glm"
IncludeDir["Box2D"] = "PrevEngine/vendor/Box2D"
IncludeDir["stb"] = "PrevEngine/vendor/stb"

include "PrevEngine/vendor/GLAD"
include "PrevEngine/vendor/ImGui"
include "PrevEngine/vendor/Box2D"
include "PrevEngine/vendor/stb"
include "PrevEngine/vendor/glm"

-- Used by both
IncludeDir["entityx"] = "PrevEngine/vendor/entityx"
include "PrevEngine/vendor/entityx"

IncludeDir["lua"] = "Sandbox/vendor/lua"
include "Sandbox/vendor/lua"

--[[
Rendering API supported	 | renderingAPI
---------------------------------------
DirectX 				 | PV_RENDERING_API_DIRECTX // currently not supported
OpenGL					 | PV_RENDERING_API_OPENGL
]]--
renderingAPI = "PV_RENDERING_API_OPENGL"

if (renderingAPI ~= "PV_RENDERING_API_OPENGL") then
	io.write("Only OpenGL supported. Use renderingAPI = \"PV_RENDERING_API_OPENGL\"")
end

--[[
Platform supported		| platform
----------------------------------
Window					| PV_PLATFORM_WINDOWS
]]--
platform = "PV_PLATFORM_WINDOWS"

--[[
Windowing API supprted  | windowingAPI
--------------------------------------
Win32					| PV_WINDOWING_API_WIN32 -- Dosen't support imgui viewport and can be only used if PV_PLATFORM_WINDOWS
GLFW					| PV_WINDOWING_API_GLFW  -- Cross platform and support imgui viewport
]]--
windowingAPI = "PV_WINDOWING_API_GLFW"

if (windowingAPI == "PV_WINDOWING_API_GLFW") then
	IncludeDir["glfw"] = "PrevEngine/vendor/glfw/include"
	include "PrevEngine/vendor/glfw"
end

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
	
	if (windowingAPI == "PV_WINDOWING_API_GLFW") then
		removefiles {
			"%{prj.name}/src/platform/windows/windowswindow.cpp",
			"%{prj.name}/src/platform/windows/windowswindow.h",
			
			"%{prj.name}/src/platform/windows/imguiwindowsinit.cpp",
			"%{prj.name}/src/platform/windows/imguiwindowsinit.h",
		}
		
		links {
			"GLFW"
		}
		
		includedirs {
			"%{IncludeDir.glfw}",
		}
		
	else
		removefiles {
			"%{prj.name}/src/platform/glfw/**.*",
		}
	end
	
    includedirs {
        "%{prj.name}/src",
		"%{IncludeDir.GLAD}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.entityx}",
		"%{IncludeDir.Box2D}",
		"%{IncludeDir.lua}",
		"%{IncludeDir.stb}"
    }
	
	links {
		"GLAD",
		"ImGui",
		"entityx",
		"Box2D",
		"lua"
	}

    filter "system:windows"
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"
		
        defines {
            platform,
			renderingAPI,
			windowingAPI,
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
        defines {"PV_DEBUG"}
        symbols "On"

    filter "configurations:Release"
	    defines {"PV_RELEASE"}
	    optimize "On"

	filter "configurations:Distribute"
		defines {"PV_DIST"}
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
		"%{IncludeDir.glm}",
		"%{IncludeDir.entityx}",
		"%{IncludeDir.Box2D}",
		"%{IncludeDir.lua}"
    }
	
	links {
		"PrevEngine"
	}
	
	filter "system:windows"
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"

        defines {
            platform,
			windowingAPI,
			"_CRT_SECURE_NO_WARNINGS"
        }
		
		pchheader "pch.h"
		
	filter "action:vs*"
		pchsource "Sandbox/src/pch.cpp"
	
	filter "configurations:Debug"
		defines {"PV_DEBGU"}
		symbols "On"

	filter "configurations:Release"
		defines {"PV_RELEASE"}
		optimize "On"

	filter "configurations:Distribute"
		defines {"PV_DIST"}
		optimize "On"
