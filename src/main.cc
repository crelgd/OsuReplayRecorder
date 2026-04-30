#include <iostream>
#include <bitset>   
//#include "osr.h"

int uleb128_decode(std::vector<unsigned char>& bytes)
{
    int out = 0;
    int shift = 0;
    int byteSize = bytes.size();

    for (int i = 0; i < byteSize-1; i++)
    {
        char byteDecompile = bytes[i] & 0x7f;
        out |= byteDecompile << shift;

        shift += 7; 
    }

    out |= bytes[byteSize-1] << shift;

    return out;
}

int main(int argc, char* argv[])
{

    /*
    OsrFile file(argv[1]);

    int signF = file.ReadSign();

    if (signF != 1)
    {
        std::cout
            << file.sign.mode << "\n"
            << file.sign.ver << "\n"
            << file.sign.md5card << "\n"
            //<< file.sign.player << "\n"
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
    */

    std::vector<unsigned char> uleb128test = {0xE5, 0x8E, 0x26};

    int decode = uleb128_decode(uleb128test);

    std::cout << decode << std::endl;

    return 0;
}
