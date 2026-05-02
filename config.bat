@echo off

mkdir build
mkdir include
mkdir _dependence && cd _dependence

git clone https://github.com/tukaani-project/xz.git
move xz\src\liblzma\api ..\include\lzma 

cd .. && cd lib 

lib /DEF:liblzma.def /OUT:liblzma.lib

cd .. && cd build

cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build .

cd ..