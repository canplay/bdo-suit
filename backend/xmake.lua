set_project("backend")
set_languages("c17", "cxx20")
set_warnings("more")
set_defaultmode("releasedbg")
set_allowedmodes("debug", "releasedbg")

add_rules("plugin.compile_commands.autoupdate")

if is_host("windows") then
    add_requires("vcpkg::fmt", "vcpkg::spdlog", "vcpkg::nanodbc", "vcpkg::cryptopp", "vcpkg::drogon[core,orm,mysql,postgres,sqlite3,redis]", "vcpkg::trantor", "vcpkg::crashpad", "vcpkg::openssl", "vcpkg::tinyxml2", "vcpkg::jsoncpp", "vcpkg::zlib", "vcpkg::brotli", "vcpkg::jwt-cpp", "vcpkg::bshoshany-thread-pool", "vcpkg::icu", "vcpkg::libmariadb[core,schannel,ssl]", "vcpkg::libpq[core,lz4,openssl,zlib]", "vcpkg::lz4", "vcpkg::picojson", "vcpkg::sqlite3[core,json1]", "vcpkg::hiredis", "vcpkg::c-ares", { configs = { shared = true } })
else
    add_requires("vcpkg::fmt", "vcpkg::spdlog", "vcpkg::nanodbc", "vcpkg::cryptopp", "vcpkg::libuuid", "vcpkg::drogon[core,orm,mysql,postgres,sqlite3,redis]", "vcpkg::trantor", "vcpkg::openssl", "vcpkg::tinyxml2", "vcpkg::jsoncpp", "vcpkg::zlib", "vcpkg::brotli", "vcpkg::jwt-cpp", "vcpkg::bshoshany-thread-pool", "vcpkg::icu", "vcpkg::libmariadb[core,schannel,ssl]", "vcpkg::libpq[core,lz4,openssl,zlib]", "vcpkg::lz4", "vcpkg::picojson", "vcpkg::sqlite3[core,json1]", "vcpkg::hiredis", "vcpkg::c-ares")
end

add_defines("_CRT_SECURE_NO_WARNINGS", "NOMINMAX", "NANODBC_USE_UNICODE")

local path_fix = ""
if is_mode("debug") then
    path_fix = "debug"
else
    path_fix = "release"
end 

target("backend")
    set_kind("binary")
    set_plat(os.host())
    set_arch(os.arch())
    set_targetdir("$(buildir)/$(host)/x64/"..path_fix.."/backend")
    set_pcxxheader("include/pch.h")

    if is_mode("debug") then
        set_symbols("debug")
        set_optimize("none")
    else
        set_symbols("debug")
        set_optimize("fastest")
        set_strip("all")
    end

    if is_host("windows") then
        set_encodings("utf-8")
        set_runtimes("MD")
        add_includedirs("F:/Code/vcpkg/installed/x64-windows/include/crashpad")
        add_syslinks("Advapi32", "odbc32")
        add_cxxflags("/wd4819")
        add_packages("vcpkg::fmt", "vcpkg::spdlog", "vcpkg::nanodbc", "vcpkg::cryptopp", "vcpkg::drogon[core,orm,mysql,postgres,sqlite3,redis]", "vcpkg::trantor", "vcpkg::crashpad", "vcpkg::openssl", "vcpkg::tinyxml2", "vcpkg::jsoncpp", "vcpkg::zlib", "vcpkg::brotli", "vcpkg::jwt-cpp", "vcpkg::bshoshany-thread-pool", "vcpkg::icu", "vcpkg::libmariadb[core,schannel,ssl]", "vcpkg::libpq[core,lz4,openssl,zlib]", "vcpkg::lz4", "vcpkg::picojson", "vcpkg::sqlite3[core,json1]", "vcpkg::hiredis", "vcpkg::c-ares")
    else
        set_toolchains("llvm")
        add_syslinks("uuid")
        add_packages("vcpkg::fmt", "vcpkg::spdlog", "vcpkg::nanodbc", "vcpkg::cryptopp", "vcpkg::libuuid", "vcpkg::drogon[core,orm,mysql,postgres,sqlite3,redis]", "vcpkg::trantor", "vcpkg::openssl", "vcpkg::tinyxml2", "vcpkg::jsoncpp", "vcpkg::zlib", "vcpkg::brotli", "vcpkg::jwt-cpp", "vcpkg::bshoshany-thread-pool", "vcpkg::icu", "vcpkg::libmariadb[core,schannel,ssl]", "vcpkg::libpq[core,lz4,openssl,zlib]", "vcpkg::lz4", "vcpkg::picojson", "vcpkg::sqlite3[core,json1]", "vcpkg::hiredis", "vcpkg::c-ares")
    end
    
    add_includedirs("common/include")
    add_files("common/src/*.cpp")
    add_includedirs("include")
    add_files("src/*.cpp")

    after_build(function (target)
        if is_host("windows") then
            os.cp("F:/Code/vcpkg/installed/x64-windows/bin/brotlicommon.dll", "$(buildir)/$(host)/x64/"..path_fix.."/"..target:name())
            os.cp("F:/Code/vcpkg/installed/x64-windows/bin/brotlidec.dll", "$(buildir)/$(host)/x64/"..path_fix.."/"..target:name())
            os.cp("F:/Code/vcpkg/installed/x64-windows/bin/brotlienc.dll", "$(buildir)/$(host)/x64/"..path_fix.."/"..target:name())
            os.cp("F:/Code/vcpkg/installed/x64-windows/bin/cares.dll", "$(buildir)/$(host)/x64/"..path_fix.."/"..target:name())
            os.cp("F:/Code/vcpkg/installed/x64-windows/bin/drogon.dll", "$(buildir)/$(host)/x64/"..path_fix.."/"..target:name())
            os.cp("F:/Code/vcpkg/installed/x64-windows/bin/fmt.dll", "$(buildir)/$(host)/x64/"..path_fix.."/"..target:name())
            os.cp("F:/Code/vcpkg/installed/x64-windows/bin/hiredis.dll", "$(buildir)/$(host)/x64/"..path_fix.."/"..target:name())
            os.cp("F:/Code/vcpkg/installed/x64-windows/bin/jsoncpp.dll", "$(buildir)/$(host)/x64/"..path_fix.."/"..target:name())
            os.cp("F:/Code/vcpkg/installed/x64-windows/bin/libcrypto-3-x64.dll", "$(buildir)/$(host)/x64/"..path_fix.."/"..target:name())
            os.cp("F:/Code/vcpkg/installed/x64-windows/bin/libecpg.dll", "$(buildir)/$(host)/x64/"..path_fix.."/"..target:name())
            os.cp("F:/Code/vcpkg/installed/x64-windows/bin/libecpg_compat.dll", "$(buildir)/$(host)/x64/"..path_fix.."/"..target:name())
            os.cp("F:/Code/vcpkg/installed/x64-windows/bin/libmariadb.dll", "$(buildir)/$(host)/x64/"..path_fix.."/"..target:name())
            os.cp("F:/Code/vcpkg/installed/x64-windows/bin/libpgtypes.dll", "$(buildir)/$(host)/x64/"..path_fix.."/"..target:name())
            os.cp("F:/Code/vcpkg/installed/x64-windows/bin/libpq.dll", "$(buildir)/$(host)/x64/"..path_fix.."/"..target:name())
            os.cp("F:/Code/vcpkg/installed/x64-windows/bin/libssl-3-x64.dll", "$(buildir)/$(host)/x64/"..path_fix.."/"..target:name())
            os.cp("F:/Code/vcpkg/installed/x64-windows/bin/lz4.dll", "$(buildir)/$(host)/x64/"..path_fix.."/"..target:name())
            os.cp("F:/Code/vcpkg/installed/x64-windows/bin/spdlog.dll", "$(buildir)/$(host)/x64/"..path_fix.."/"..target:name())
            os.cp("F:/Code/vcpkg/installed/x64-windows/bin/sqlite3.dll", "$(buildir)/$(host)/x64/"..path_fix.."/"..target:name())
            os.cp("F:/Code/vcpkg/installed/x64-windows/bin/tinyxml2.dll", "$(buildir)/$(host)/x64/"..path_fix.."/"..target:name())
            os.cp("F:/Code/vcpkg/installed/x64-windows/bin/trantor.dll", "$(buildir)/$(host)/x64/"..path_fix.."/"..target:name())
            os.cp("F:/Code/vcpkg/installed/x64-windows/bin/zlib1.dll", "$(buildir)/$(host)/x64/"..path_fix.."/"..target:name())
            os.cp("F:/Code/vcpkg/installed/x64-windows/bin/icuuc73.dll", "$(buildir)/$(host)/x64/"..path_fix.."/"..target:name())
            os.cp("F:/Code/vcpkg/installed/x64-windows/bin/icudt73.dll", "$(buildir)/$(host)/x64/"..path_fix.."/"..target:name())
            os.cp("F:/Code/vcpkg/installed/x64-windows/tools/crashpad_handler.exe", "$(buildir)/$(host)/x64/"..path_fix.."/"..target:name())
        end
        
        os.cp("icudt73l.dat", "$(buildir)/$(host)/x64/"..path_fix.."/"..target:name())
        os.cp("config.json", "$(buildir)/$(host)/x64/"..path_fix.."/"..target:name())
    end)
