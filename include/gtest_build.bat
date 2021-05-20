
@echo off
set PROJECT_TYPE="Visual Studio 16 2019"
set SRC_DIR=C:\work\gtest\googletest-1.10.x

rem  supported platforms:
rem  Win32
rem  x64
set PLATFORM=Win32

set BUILD_DIR="build-x86"
set PREFIX=c:\lib\gtest
set BUILD_TYPE=Debug

echo.
echo Configure Google Test library...
cmake ^
    -G %PROJECT_TYPE% ^
    -A %PLATFORM% ^
    -S %SRC_DIR% ^
    -B %BUILD_DIR% ^
    -DCMAKE_INSTALL_PREFIX=%PREFIX%
if %ERRORLEVEL% == 0 goto :go_build
echo "Errors encountered during configuration. Exited with status: %errorlevel%"
goto :go_error

:go_build
echo.
echo Build Google Test library...
cmake --build %BUILD_DIR% --config %BUILD_TYPE%
if %ERRORLEVEL% == 0 goto :go_install
echo "Errors encountered during build. Exited with status: %errorlevel%"
goto :go_error

:go_install
echo.
echo Install Google Test library...
cmake --install %BUILD_DIR% --config %BUILD_TYPE%
if %ERRORLEVEL% == 0 goto :go_success
echo "Errors encountered during install. Exited with status: %errorlevel%"
goto :go_error

:go_error
echo.
echo Google Test library build failed!
exit /B %ERRORLEVEL%

:go_success
echo.
echo Google Test library successfully built and installed!
exit /B
