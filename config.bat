@echo off

set HOME=%cd%
set BIN_DIR=%HOME%\build\bin
set LICENSE_DIR=%BIN_DIR%\licenses

mkdir build include lib
mkdir build\bin\licenses
mkdir _dependence && cd _dependence

xcopy %HOME%\LICENSE %BIN_DIR%\. /I

curl -L https://github.com/tukaani-project/xz/releases/download/v5.8.3/xz-5.8.3-windows.zip -o xz.zip
tar -f xz.zip -x 
mkdir %HOME%\include\lzma
xcopy include %HOME%\include\lzma /I /S
lib /DEF:doc\liblzma.def /OUT:%HOME%\lib\liblzma.lib

git clone https://github.com/gabime/spdlog.git
cd spdlog && mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release .. && cmake --build .
cd ..
move build\spdlog.lib %HOME%\lib\.
move include\spdlog %HOME%\include\ 
move LICENSE %LICENSE_DIR%\LICENSE.spdlog
cd ..

curl -L https://github.com/nigels-com/glew/releases/download/glew-2.3.1/glew-2.3.1-win32.zip -o glew.zip
tar -f glew.zip -x
move glew-2.3.1\include\GL %HOME%\include\.
move glew-2.3.1\LICENSE.txt %HOME%\build\bin\licenses\LICENSE.glew

curl -L https://github.com/libsdl-org/SDL/releases/download/release-2.32.10/SDL2-devel-2.32.10-VC.zip -o sdl.zip 
tar -f sdl.zip -x
IF EXIST %HOME%\include\SDL (
    xcopy SDL2-2.32.10\include\* /I
) ELSE move SDL2-2.32.10\include %HOME%\include\SDL 
move SDL2-2.32.10\README-SDL.txt %LICENSE_DIR%\.
move SDL2-2.32.10\LICENSE.txt %LICENSE_DIR%\LICENSE.sdl

curl -L https://github.com/g-truc/glm/releases/download/1.0.3/glm-1.0.3.7z -o glm.zip
tar -f glm.zip -x 
xcopy glm\glm %HOME%\include\GL\glm /I /S
xcopy glm\copying.txt %LICENSE_DIR%\LICENSE.glm

IF "%PROCESSOR_ARCHITECTURE%"=="AMD64" (
    xcopy glew-2.3.1\lib\Release\x64\. %HOME%\lib\. /I 
    xcopy glew-2.3.1\bin\Release\x64\glew32.dll %BIN_DIR%\.  /I 

    xcopy SDL2-2.32.10\lib\x64\*.lib %HOME%\lib\.  /I 
    xcopy SDL2-2.32.10\lib\x64\SDL2.dll %BIN_DIR%\.  /I  

    xcopy bin_x86-64\liblzma.dll %BIN_DIR%\. /I 
) ELSE (
    xcopy glew-2.3.1\lib\Release\Win32\. %HOME%\lib\. /I 
    xcopy glew-2.3.1\bin\Release\Win32\glew32.dll %BIN_DIR%\.  /I 

    xcopy SDL2-2.32.10\lib\x86\*.lib %HOME%\lib\. /I 
    xcopy SDL2-2.32.10\lib\x86\SDL2.dll %BIN_DIR%\. /I 

    xcopy bin_i686-sse2\liblzma.dll %BIN_DIR%\. /I 
)

cd .. && cd build

cmake -DCMAKE_BUILD_TYPE=Release ..

cd ..
