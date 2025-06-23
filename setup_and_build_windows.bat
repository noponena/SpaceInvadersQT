@echo off

REM === Help Message ===
if /I "%~1"=="-h"  goto :help
if /I "%~1"=="--help" goto :help
if /I "%~1"=="/?" goto :help

REM (rest of your script here)

goto :skip_help

:help
echo.
echo Usage: %~nx0 [--clean] [core_count]
echo.
echo   --clean       Optional: Clean all build directories before building.
echo   core_count    Optional: Number of CPU cores to use for parallel builds.
echo                 If omitted, core count is auto-detected.
echo.
echo Examples:
echo   %~nx0
echo   %~nx0 8
echo   %~nx0 --clean
echo   %~nx0 --clean 12
echo.
exit /b 0

:skip_help

setlocal EnableDelayedExpansion

REM === Argument Parsing and Core Detection ===

set "CLEAN_BUILD=0"
set "CORES="

REM Detect if first argument is --clean
if /i "%~1"=="--clean" (
    set "CLEAN_BUILD=1"
    if not "%~2"=="" (
        set "CORES=%~2"
    )
) else if not "%~1"=="" (
    set "CORES=%~1"
)

REM If CORES is not set, auto-detect it
if "%CORES%"=="" (
    for /f "skip=2 tokens=2 delims=," %%i in ('wmic cpu get NumberOfLogicalProcessors /format:csv') do (
        if not "%%i"=="" (
            set "CORES=%%i"
            goto :after_core_detect
        )
    )
)
:after_core_detect

echo Using %CORES% parallel jobs for builds.

REM ==== CONFIGURATION ====
set "RESULT_DIR=build_result"

REM ==== Install OpenAL runtime ====
if exist "Thirdparty\OpenAL\redist\oalinst.exe" (
    "Thirdparty\OpenAL\redist\oalinst.exe" /S
)

REM ==== Build yaml-cpp ====
cd Thirdparty\yaml-cpp

REM === Clean build directory if requested ===
if !CLEAN_BUILD! EQU 1 (
    if exist build rmdir /s /q build
)
if not exist build mkdir build
cd build

cmake .. -G "MinGW Makefiles"
if %ERRORLEVEL% NEQ 0 goto error
cmake --build . --config Release -- -j!CORES!
if %ERRORLEVEL% NEQ 0 goto error
cd ..\..\..

REM ==== Prepare Result/Output Directory ====
if not exist "%RESULT_DIR%" mkdir "%RESULT_DIR%"

REM ==== Build Space Invaders ====
qmake -o "%RESULT_DIR%\Makefile" SpaceInvaders.pro
if %ERRORLEVEL% NEQ 0 goto error

cd "%RESULT_DIR%"
mingw32-make -j!CORES!
if %ERRORLEVEL% NEQ 0 goto error
cd ..

echo Build succeeded!
goto :eof

:error
echo.
echo ERROR: Build failed!
exit /b 1

endlocal
