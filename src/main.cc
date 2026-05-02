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

    int signF = file.Read();

    if (signF != 1)
    {
        std::cout
            << "MODE: " << file.sign.mode << "\n"
            << "VER: " << file.sign.ver << "\n"
            << "MD5CARD: " << file.sign.md5card.c_str() << "\n"
            << "PLAYER: " << file.sign.player.c_str() << "\n"
            << "MD5REPLAY: " << file.sign.md5replay.c_str() << "\n"
            << "300: " << file.sign.r300 << "\n"
            << "100: " << file.sign.r100 << "\n"
            << "50: " << file.sign.r50 << "\n"
            << "iCombos: " << file.sign.iCombos << "\n"
            << "niCombos: " << file.sign.niCombos << "\n"
            << "MISSES: " << file.sign.misses << "\n"
            << "POINTS: " << file.sign.points << "\n"
            << "MAX COMBO: " << file.sign.maxCombo << "\n"
            << "IICOMBOS: " << file.sign.iiCombos << "\n"
            << "MODE: " << file.sign.modes << "\n"
            << "HP: " << file.sign.hp.c_str() << "\n"
            << "TIME: " << file.sign.time << "\n"
            << "COMP DATA SIZE: " << file.sign.compData << "\n"
            << "RES ID: " << file.sign.resId << "\n"
            << std::endl;
    } 

    return 0;
}
