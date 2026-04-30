#include <iostream>
//#include "osr.h"

std::vector<char> uleb128_encode(int val)
{
    int shift = 0;
    std::vector<char> arr;

    while (val != 0)
    {
        int byte = val & 0x7f;
        val >>= 7;

        if (val != 0)
        {
            byte |= 0x80;
            std::cout << "wew " << byte << std::endl;
        }

        arr.push_back(char(byte));
    }

    return arr;
}

int main(int argc, char* argv[])
{
    if (argc < 2) return 0;

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

    std::vector<char> uleb128test = uleb128_encode(atoi(argv[1]));

    for (int i = 0; i < uleb128test.size(); i++)
    {
        std::cout << "0x" << std::hex << int(uleb128test[i]) << " ";
    }

    std::cout << std::endl;

    return 0;
}
