project "GLAD"
    kind "StaticLib"
    language "C"
    
	targetdir ("bin/" .. outputDir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputDir .. "/%{prj.name}")

	files {
        "include/glad/glad.h",
        "include/glad/glad_wgl.h",
        "include/KHR/khrplatform.h",
		"source/glad.c",
		"source/glad_wgl.c"
    }
    
	includedirs {
		"include"
	}
	
	filter "system:windows"
        systemversion "latest"
        staticruntime "On"
        
    filter { "system:windows", "configurations:Release" }
        buildoptions "/MT"
