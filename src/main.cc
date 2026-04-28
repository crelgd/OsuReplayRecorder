#include <iostream>
#include "osr.h"

int main(int argc, char* argv[])
{
    if (argc < 2) return 0;

    OsrFile file(argv[1]);

    int signF = file.ReadSign();

    if (signF != 1)
    {
        std::cout
            << file.sign.mode << "\n"
            << file.sign.ver << "\n"
            << file.sign.md5card << "\n"
            << file.sign.player << "\n"
            << file.sign.md5replay << "\n"
            << file.sign.r300 << "\n"
            << file.sign.r100 << "\n"
            << file.sign.r50 << "\n"
            << file.sign.iCombos << "\n"
            << file.sign.niCombos << "\n"
            << file.sign.misses << "\n"
            << file.sign.points << "\n"
            << file.sign.maxCombo << "\n"
            << file.sign.iiCombos << "\n"
            << file.sign.mode << "\n"
            << file.sign.hp << "\n"
            << file.sign.time << "\n"
            << file.sign.compData
            << std::endl;
    } 

    return 0;
}
