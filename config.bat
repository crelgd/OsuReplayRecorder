@echo off

mkdir build
mkdir include
mkdir _dependence && cd _dependence

git clone https://github.com/tukaani-project/xz.git
move xz\src\liblzma\api ..\include\lzma 

git clone https://github.com/gabime/spdlog.git
cd spdlog && mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release .. && cmake --build .
cd ..
move build\spdlog.lib ..\..\lib\.
move include\spdlog ..\..\include\.

cd .. && cd lib 

lib /DEF:liblzma.def /OUT:liblzma.lib

cd .. && cd build

cmake -DCMAKE_BUILD_TYPE=Release ..

cd ..