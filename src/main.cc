/*
 * MIT License
 * Copyright (c) 2026 crelgd
 */

#include <iostream>
//#include <bitset>   
#include "osr.h"

int main(int argc, char* argv[])
{

    OsrFile file(argv[1]);

    int signF = file.ReadSign();

    if (signF != 1)
    {
        std::cout
            << "MODE: " << int(file.sign.mode) << "\n"
            << "VER: " << file.sign.ver << "\n"
            << "MD5CARD: " << file.sign.md5card << "\n"
            << "PLAYER: " << file.sign.player << "\n"
            << "MD5REPLAY: " << file.sign.md5replay << "\n"
            << "300: " << file.sign.r300 << "\n"
            << "100: " << file.sign.r100 << "\n"
            << "50: " << file.sign.r50 << "\n"
            << "iCombos: " << file.sign.iCombos << "\n"
            << "niCombos: " << file.sign.niCombos << "\n"
            << "MISSES: " << file.sign.misses << "\n"
            << "POINTS: " << int(file.sign.points) << "\n"
            << "MAX COMBO: " << int(file.sign.maxCombo) << "\n"
            << "IICOMBOS: " << int(file.sign.iiCombos) << "\n"
            << "MODE: " << file.sign.modes << "\n"
            << "HP: " << file.sign.hp << "\n"
            << "TIME: " << int64_t(file.sign.time) << "\n"
            << "COMP DATA SIZE: " << file.sign.compData
            << std::endl;
    } 

    return 0;
}
