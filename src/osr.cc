/*
 * MIT License
 * Copyright (c) 2026 crelgd
 */

#include "osr.h"

#include <windows.h>
#include <fstream>
#include <ios>
#include <iostream>

OsrFile::OsrFile(std::string fName)
{
    std::ifstream file(fName, std::ios::in | std::ios::binary);

    file.seekg(0, std::ios::end);
    fSize = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<char> tv(fSize);
    file.read(TYPE<char*>(tv.data()), fSize);

    fileData = tv;
}

int OsrFile::ReadSign()
{
    if (fileData.size() < fSize)
    {
        return 1;
    }

    ReadStruct();

    return 0;
}

std::string OsrFile::GetString(int lebSize)
{
    unsigned char byte = GetVal<unsigned char>();
    std::string str;
    
    switch (byte)
    {
    case 0x00:
        {
            return {};
        }
        break;

    case 0x0B:
        {
            int strSize = uleb128_decode();
            str.resize(strSize+1);
        
            memcpy(&str[0], fileData.data() + offset, strSize);
            offset += strSize;

            str[strSize] = '\0';
        }
        break;
    }

    return str;
}

void OsrFile::ReadStruct()
{
    sign.mode       = GetVal<unsigned char>();
    sign.ver        = GetVal<int>();
    sign.md5card    = GetString(1).data();
    sign.player     = GetString(1).data();
    sign.md5replay  = GetString(1).data();
    sign.r300       = GetVal<short>();
    sign.r100       = GetVal<short>();
    sign.r50        = GetVal<short>();
    sign.iCombos    = GetVal<short>();
    sign.niCombos   = GetVal<short>();
    sign.misses     = GetVal<short>();
    sign.points     = GetVal<int>();
    sign.maxCombo   = GetVal<short>();
    sign.iiCombos   = GetVal<unsigned char>();
    sign.modes      = GetVal<int>();
    sign.hp         = GetString(1).data();
    sign.time       = GetVal<int64_t>();
    sign.compData   = GetVal<int>();
}   

int OsrFile::uleb128_decode()
{
    int out = 0;
    int shift = 0;
    char byte;

    do
    {
        byte =  GetVal<unsigned char>();
        unsigned char byteDecompile = byte & 0x7f;
        out |= byteDecompile << shift;

        shift += 7; 
    } while((byte & 0x80) != 0);

    return out;
}
std::vector<char> OsrFile::uleb128_encode(int val)
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
        }

        arr.push_back(char(byte));
    }

    return arr;
}

