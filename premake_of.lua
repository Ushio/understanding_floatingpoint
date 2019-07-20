-- Required OF_ROOT

function search_of_root()
    root = ""
    cwd = os.getcwd()

    for i = 1, 10 do
        os.chdir("..")
        if(os.isdir("addons") and os.isdir("apps") and os.isdir("docs") and os.isdir("examples") and os.isdir("libs")) then
            root = os.getcwd()
            break
        end
    end

    os.chdir(cwd)

    return root
end

function _add_of_dependency_common()
    includedirs { 
        "%{OF_ROOT}/libs/openFrameworks",
        "%{OF_ROOT}/libs/openFrameworks/graphics",
        "%{OF_ROOT}/libs/openFrameworks/app",
        "%{OF_ROOT}/libs/openFrameworks/sound",
        "%{OF_ROOT}/libs/openFrameworks/utils",
        "%{OF_ROOT}/libs/openFrameworks/communication",
        "%{OF_ROOT}/libs/openFrameworks/video",
        "%{OF_ROOT}/libs/openFrameworks/types",
        "%{OF_ROOT}/libs/openFrameworks/math",
        "%{OF_ROOT}/libs/openFrameworks/3d",
        "%{OF_ROOT}/libs/openFrameworks/gl",
        "%{OF_ROOT}/libs/openFrameworks/events",
        "%{OF_ROOT}/libs/glm/include",
        "%{OF_ROOT}/libs/rtAudio/include",
        "%{OF_ROOT}/libs/quicktime/include",
        "%{OF_ROOT}/libs/freetype/include",
        "%{OF_ROOT}/libs/freetype/include/freetype2",
        "%{OF_ROOT}/libs/freeImage/include",
        "%{OF_ROOT}/libs/fmodex/include",
        "%{OF_ROOT}/libs/videoInput/include",
        "%{OF_ROOT}/libs/glew/include/",
        "%{OF_ROOT}/libs/glu/include",
        "%{OF_ROOT}/libs/tess2/include",
        "%{OF_ROOT}/libs/cairo/include/cairo",
        "%{OF_ROOT}/libs/glfw/include",
        "%{OF_ROOT}/libs/openssl/include",
        "%{OF_ROOT}/libs/utf8/include",
        "%{OF_ROOT}/libs/boost/include",
        "%{OF_ROOT}/libs/json/include",
        "%{OF_ROOT}/libs/curl/include",
        "%{OF_ROOT}/libs/uriparser/include",
        "%{OF_ROOT}/libs/pugixml/include",
        "%{OF_ROOT}/addons",
    }

    libdirs {
        "%{OF_ROOT}/libs/glfw/lib/vs/x64",
        "%{OF_ROOT}/libs/rtAudio/lib/vs/x64",
        "%{OF_ROOT}/libs/FreeImage/lib/vs/x64",
        "%{OF_ROOT}/libs/freetype/lib/vs/x64",
        "%{OF_ROOT}/libs/fmodex/lib/vs/x64",
        "%{OF_ROOT}/libs/videoInput/lib/vs/x64",
        "%{OF_ROOT}/libs/cairo/lib/vs/x64",
        "%{OF_ROOT}/libs/glew/lib/vs/x64",
        "%{OF_ROOT}/libs/glu/lib/vs/x64",
        "%{OF_ROOT}/libs/openssl/lib/vs/x64",
        "%{OF_ROOT}/libs/curl/lib/vs/x64",
        "%{OF_ROOT}/libs/tess2/lib/vs/x64",
        "%{OF_ROOT}/libs/boost/lib/vs/x64",
        "%{OF_ROOT}/libs/uriparser/lib/vs/x64",
        "%{OF_ROOT}/libs/pugixml/lib/vs/x64",
    }

    -- DLLs
    matches = os.matchfiles(path.join(OF_ROOT, "libs/**.dll"))
    for i, file in pairs(matches) do
        postbuildcommands {
            "{COPY} " .. file .. " %{cfg.targetdir}"
        }
    end
end

function add_of_dependency_debug()
    _add_of_dependency_common()

    links { 
        "kernel32",
        "user32",
        "gdi32",
        "winspool",
        "comdlg32",
        "advapi32",
        "shell32",
        "ole32",
        "oleaut32",
        "uuid",
        "odbc32",
        "odbccp32",
        "wldap32",

        "cairo-static",
        "pixman-1",
        "libpng",
        "zlib",
        "msimg32",
        "OpenGL32",
        "kernel32",
        "setupapi",
        "Vfw32",
        "comctl32",
        "rtAudioD",
        "videoInputD",
        "libfreetype",
        "FreeImage",
        "dsound",
        "user32",
        "gdi32",
        "winspool",
        "comdlg32",
        "advapi32",
        "shell32",
        "ole32",
        "oleaut32",
        "uuid",
        "glew32s",
        "fmodex64_vc",
        "libssl",
        "libcrypto",
        "crypt32",
        "libcurl",
        "uriparser",
        "pugixmld",
        "Ws2_32",
        "tess2",
        "glfw3",
        "winmm",
    }

    defines {
        "WIN32",
        "_DEBUG",
        "_CONSOLE",
        "POCO_STATIC",
        "CAIRO_WIN32_STATIC_BUILD",
        "DISABLE_SOME_FLOATING_POINT",
    }
end

function add_of_dependency_release()
    _add_of_dependency_common()

    links { 
        "kernel32",
        "user32",
        "gdi32",
        "winspool",
        "comdlg32",
        "advapi32",
        "shell32",
        "ole32",
        "oleaut32",
        "uuid",
        "odbc32",
        "odbccp32",
        "wldap32",

        "cairo-static",
        "pixman-1",
        "libpng",
        "zlib",
        "msimg32",
        "OpenGL32",
        "GLu32",
        "kernel32",
        "setupapi",
        "Vfw32",
        "comctl32",
        "rtAudio",
        "videoInput",
        "libfreetype",
        "FreeImage",
        "dsound",
        "user32",
        "gdi32",
        "winspool",
        "comdlg32",
        "advapi32",
        "shell32",
        "ole32",
        "oleaut32",
        "uuid",
        "glew32s",
        "fmodex64_vc",
        "libssl",
        "libcrypto",
        "crypt32",
        "libcurl",
        "uriparser",
        "pugixml",
        "Ws2_32",
        "tess2",
        "glfw3",
        "winmm",
    }

    defines {
        "WIN32",
        "NDEBUG",
        "_CONSOLE",
        "POCO_STATIC",
        "CAIRO_WIN32_STATIC_BUILD",
        "DISABLE_SOME_FLOATING_POINT",
    }
end



