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
IncludeDir["GLAD"] 	= "PrevEngine/vendor/GLAD/include"
IncludeDir["ImGui"] = "PrevEngine/vendor/ImGui"
IncludeDir["glm"] 	= "PrevEngine/vendor/glm/glm"
IncludeDir["Box2D"] = "PrevEngine/vendor/Box2D"
IncludeDir["glfw"] = "PrevEngine/vendor/glfw/include"

include "PrevEngine/vendor/GLAD"
include "PrevEngine/vendor/ImGui"
include "PrevEngine/vendor/glm"
include "PrevEngine/vendor/Box2D"
include "PrevEngine/vendor/glfw"

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
linux					| PV_PLATFORM_LINUX
]]--
platform = "PV_PLATFORM_WINDOWS"

--[[
Windowing API supprted  | windowingAPI
--------------------------------------
Win32					| PV_WINDOWING_API_WIN32 -- Dosen't support imgui viewport and can be only used if PV_PLATFORM_WINDOWS
X11						| PV_WINDOWING_API_X11	 -- Dosen't support imgui viewport and can be only used if PV_PLATFORM_LINUX
GLFW					| PV_WINDOWING_API_GLFW  -- Cross platform and support imgui viewport
]]--
windowingAPI = "PV_WINDOWING_API_GLFW"

if (windowingAPI == "PV_WINDOWING_API_WIN32" and platform == "PV_PLATFORM_LINUX") then
	io.write("Cannot use Win32 for Linux")
end

if (windowingAPI == "PV_WINDOWING_API_X11" and platform == "PV_PLATFORM_WINDOWS") then
	io.write("Cannot use X11 for Windows")
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
			
			"%{prj.name}/src/platform/linux/linuxwindow.cpp",
			"%{prj.name}/src/platform/linux/linuxwindow.h",
		}
		
		links {
			"GLFW"
		}
	end
	
    includedirs {
        "%{prj.name}/src",
		"%{IncludeDir.GLAD}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.entityx}",
		"%{IncludeDir.Box2D}",
		"%{IncludeDir.glfw}",
		"%{IncludeDir.lua}"
    }
	
	links {
		"GLAD",
		"ImGui",
		"entityx",
		"Box2D",
		"lua"
	}

	filter "system:linux"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"
		
		if (renderingAPI ~= "PV_RENDERING_API_OPENGL" and platform == "PV_PLATFORM_LINUX") then
			error("Can's use directx on linux")
		end
		
		if (windowingAPI == "PV_WINDOWING_API_WIN32" and platform == "PV_PLATFORM_LINUX") then
			error("Can't use win32 on linux")
		end
		
		defines {
            platform,
			windowingAPI,
			renderingAPI,
            "PV_BUILD_LIB",
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

		if (windowingAPI == "PV_WINDOWING_API_X11" and platform == "PV_PLATFORM_WINDOWS") then
			error("Can't use x11 on windows")
		end
		
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
		"%{IncludeDir.glm}",
		"%{IncludeDir.entityx}",
		"%{IncludeDir.Box2D}",
		"%{IncludeDir.lua}"
    }
	
	links {
		"PrevEngine"
	}
	
	filter "system:linux"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines {
            platform,
			windowingAPI
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
			windowingAPI,
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
