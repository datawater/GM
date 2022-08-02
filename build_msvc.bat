@echo off
rem launch this from msvs-enabled console

set CXXFLAGS=/std:c11 /O2 /FC /Zi /W4 /WX /wd4458 /wd4996 /nologo
set LIBS=Shell32.lib

cl.exe %CXXFLAGS% %INCLUDES% /Fe"gasm.debug.exe" src/gasm.c ^
    /link %LIBS% -SUBSYSTEM:windows
cl.exe %CXXFLAGS% %INCLUDES% /Fe"gasm.debug.exe" src/gm.c ^
    /link %LIBS% -SUBSYSTEM:windows

rem TODO: Implement a release build for MSVC
rem TODO: Implement tests for MSVC.