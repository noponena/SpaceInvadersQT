@echo off
setlocal EnableDelayedExpansion

rem Install OpenAL runtime
if exist "Thirdparty\OpenAL\redist\oalinst.exe" (
    "Thirdparty\OpenAL\redist\oalinst.exe" /S
)

rem Build yaml-cpp
cd Thirdparty\yaml-cpp
if not exist build mkdir build
cd build
cmake .. -A x64
cmake --build . --config Release
cd ..\..\..

rem Build Space Invaders
qmake SpaceInvaders.pro
make

endlocal
