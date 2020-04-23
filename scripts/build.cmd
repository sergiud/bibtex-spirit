cd c:\projects\bibtex-spirit
mkdir build
cd build

if "%PLATFORM%" == "x86" (
    set ARCH=Win32
    set lib=lib32
) else (
    set ARCH=%PLATFORM%
    set lib=lib64
)

if "%APPVEYOR_BUILD_WORKER_IMAGE%" == "Visual Studio 2019" (
    goto msvc2019
)
if "%APPVEYOR_BUILD_WORKER_IMAGE%" == "Visual Studio 2017" (
    goto msvc2017
)
if "%APPVEYOR_BUILD_WORKER_IMAGE%" == "Visual Studio 2015" (
    goto msvc2015
)
if "%APPVEYOR_BUILD_WORKER_IMAGE%" == "Visual Studio 2013" (
    goto msvc2013
)

:msvc2019
set BOOST_ROOT=C:\Libraries\boost_1_71_0
set "PATH=%BOOST_ROOT%\%lib%-msvc-14.2;%PATH%"
goto build

:msvc2017
set BOOST_ROOT=C:\Libraries\boost_1_69_0
set "PATH=%BOOST_ROOT%\%lib%-msvc-14.1;%PATH%"
goto build

:msvc2015
set BOOST_ROOT=C:\Libraries\boost_1_69_0
set "PATH=%BOOST_ROOT%\%lib%-msvc-14.0;%PATH%"
goto build

:msvc2013
set BOOST_ROOT=C:\Libraries\boost_1_58_0
set "PATH=%BOOST_ROOT%\%lib%-msvc-12.0;%PATH%"
goto build

:build

cmake .. -A %ARCH%
cmake --build . --config Debug
