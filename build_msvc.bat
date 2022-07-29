@echo off
rem launch this from msvs-enabled console


set CXXFLAGS=/std:c11 /nologo

cl.exe %CXXFLAGS% src/gasm.c
cl.exe %CXXFLAGS% src/gm.c

rem TODO: implement a release build for MSVC