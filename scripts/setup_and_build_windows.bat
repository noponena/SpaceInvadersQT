@echo off

REM === Help Message ===
if /I "%~1"=="-h"  goto :help
if /I "%~1"=="--help" goto :help
if /I "%~1"=="/?" goto :help

REM Move up one directory so script works from the 'scripts' folder
cd ..

goto :skip_help

:help
echo.
echo Usage: %~nx0 [--clean] [--cores N] 
echo.
echo   --clean                    Optional: Clean all build directories before building.
echo   --cores N, -j N            Optional: Number of CPU cores to use for parallel builds.
echo                              If omitted, the default is 8.
echo.
echo Examples:
echo   %~nx0
echo   %~nx0 --cores 8
echo   %~nx0 --clean
echo   %~nx0 --clean --cores 12
echo.
exit /b 0

:skip_help

setlocal EnableDelayedExpansion

REM === Argument Parsing ===

set "CLEAN_BUILD=0"
set "CORES=8"  REM Default to 8 if not specified

:parse_args
if "%~1"=="" goto args_parsed

if /I "%~1"=="--clean" (
    set "CLEAN_BUILD=1"
) else if /I "%~1"=="--cores" (
    shift
    if "%~1"=="" (
        echo ERROR: --cores requires a value.
        exit /b 1
    )
    set "CORES=%~1"
) else if /I "%~1"=="-j" (
    shift
    if "%~1"=="" (
        echo ERROR: -j requires a value.
        exit /b 1
    )
    set "CORES=%~1"
) else (
    REM If argument is a number, treat as core count (for backward compatibility)
    set "IS_NUM=0"
    for /f "delims=0123456789" %%c in ("%~1") do set IS_NUM=1
    if "!IS_NUM!"=="0" set "CORES=%~1"
)
shift
goto :parse_args
:args_parsed

echo Using %CORES% parallel jobs for builds.

REM ==== CONFIGURATION ====

REM ==== Set build result directory name ====
set "RESULT_DIR=script_build_result"

REM ==== Clean build result if requested ====
if !CLEAN_BUILD! EQU 1 (
    echo Cleaning previous build result: %RESULT_DIR%
    if exist "%RESULT_DIR%" rmdir /s /q "%RESULT_DIR%"
)

REM ==== Warn about OpenAL runtime ====
if not exist "%WINDIR%\System32\OpenAL32.dll" (
    echo.
    echo WARNING: OpenAL runtime may not be installed on your system.
    echo Please run Thirdparty\OpenAL\redist\oalinst.exe manually if you encounter audio issues.
    echo.
)

REM ==== Build yaml-cpp ====
cd Thirdparty\yaml-cpp

REM === Clean build directory if requested ===
if !CLEAN_BUILD! EQU 1 (
    if exist build rmdir /s /q build
)
if not exist build mkdir build
cd build

cmake .. -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release
if %ERRORLEVEL% NEQ 0 goto error
cmake --build . --config Release -- -j!CORES!
if %ERRORLEVEL% NEQ 0 goto error
cd ..\..\..

REM ==== Prepare Result/Output Directory ====
if not exist "%RESULT_DIR%" mkdir "%RESULT_DIR%"

REM ==== Build Space Invaders with CMake ====
cd "%RESULT_DIR%"

if !CLEAN_BUILD! EQU 1 (
    for /d %%i in (*) do rmdir /s /q "%%i"
    for %%i in (*) do del /f /q "%%i"
)

cmake .. -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release
if %ERRORLEVEL% NEQ 0 goto error

cmake --build . --config Release -- -j!CORES!
if %ERRORLEVEL% NEQ 0 goto error
cd ..

REM ==== Always remove the debug folder from build result ====
set "DEBUG_DIR=%RESULT_DIR%\Debug"
if exist "%DEBUG_DIR%" (
    rmdir /s /q "%DEBUG_DIR%"
    echo Removed debug folder: %DEBUG_DIR%
)

echo Build succeeded!
pause
goto :eof

:error
echo.
echo ERROR: Build failed!
pause
exit /b 1

endlocal
