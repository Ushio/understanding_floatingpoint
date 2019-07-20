include "premake_of.lua"

newoption {
    trigger = "OF_ROOT",
    value = "",
    description = "Choose openframeworks dir",
}

if(_OPTIONS["OF_ROOT"]) then
    OF_ROOT = _OPTIONS["OF_ROOT"]
else
    OF_ROOT = search_of_root()
end

print ("OF_ROOT = " .. OF_ROOT)

workspace "FloatingPointStudy"
    location "build"
    configurations { "Debug", "Release" }

architecture "x86_64"

externalproject "openframeworksLib"
    location "%{OF_ROOT}/libs/openFrameworksCompiled/project/vs/" 
    kind     "StaticLib"
    language "C++"

project "window"
    kind "ConsoleApp"
    language "C++"
    targetdir "build/window_bin"
    systemversion "latest"
    
    files { "window_src/**.h", "window_src/**.hpp", "window_src/**.cpp" }

    includedirs { "common/" }
    files { "common/**.hpp", "common/**.cpp" }
    
    flags { "MultiProcessorCompile", "NoPCH" }

    dependson { "openframeworksLib" }
    links { "openframeworksLib" }

    symbols "On"
    buildoptions { "/utf-8" }

    APP_NAME = "window"
    filter {"Debug"}
        targetname ("%{APP_NAME}_Debug")
        optimize "Off"
        add_of_dependency_debug()
    filter {"Release"}
        targetname ("%{APP_NAME}")
        optimize "Full"
        add_of_dependency_release()
        
project "unit_test"
    kind "ConsoleApp"
    language "C++"
    targetdir "build/unit_test_bin"
    systemversion "latest"

    files { "unit_test_src/**.h", "unit_test_src/**.hpp", "unit_test_src/**.cpp" }

    includedirs { "common/" }
    files { "common/**.hpp", "common/**.cpp" }

    -- glm
    includedirs { "libs/glm/" }
    defines {"GLM_FORCE_CTOR_INIT"}

    symbols "On"
    buildoptions { "/utf-8" }

    APP_NAME = "unit_test"
    filter {"Debug"}
        targetname ("%{APP_NAME}_Debug")
        optimize "Off"
    filter {"Release"}
        targetname ("%{APP_NAME}")
        optimize "Full"