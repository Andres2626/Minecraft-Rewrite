@echo off
setlocal enabledelayedexpansion
set file=build.h
set /p line=<%file%
for /f "tokens=3" %%a in ("%line%") do set /a count=%%a + 1
echo #define BUILD_COUNT !count! > %file%