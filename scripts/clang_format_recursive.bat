@echo off
REM Go one level up from the script folder
set "PROJECT_ROOT=%~dp0.."
for %%I in ("%PROJECT_ROOT%") do set "PROJECT_ROOT=%%~fI"

echo Formatting C/C++ files under: %PROJECT_ROOT%
echo Excluding all folders with 'Thirdparty' or 'build' in their path.

REM Loop through all relevant files
for /r "%PROJECT_ROOT%" %%F in (*.h *.hpp *.c *.cpp) do (
    set "file=%%F"
    call :ShouldSkip "%%F"
    if errorlevel 1 (
        REM skip file
        REM do nothing
    ) else (
        echo Formatting: %%F
        clang-format -i "%%F"
    )
)
echo Formatting complete!
pause
goto :eof

:ShouldSkip
setlocal EnableDelayedExpansion
set "p=%~1"
REM Change both to lowercase for case-insensitive comparison
set "p=!p:~0!"
set "lowerp=!p!"
for %%X in (build Build BUILD thirdparty Thirdparty THIRDparty THIRDParty THIRDPARTY) do (
    echo !lowerp! | findstr /I "%%X" >nul
    if not errorlevel 1 (
        endlocal
        exit /b 1
    )
)
endlocal
exit /b 0
