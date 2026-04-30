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

// этат пиздец лучше не трогать ваще
//  хазе но вроде может память сламать
// бля нужна ваще не массив передавать а бля в int хуярить данные через memcpy
// кароч переделать кадата
void OsrFile::GetVal(void* dst, char type)
{
    char bReaderSize = sizeof(bReaderFormat) / sizeof(bReaderFormat[0]);

    for (int i = 0; i < bReaderSize; i++)
    {
        char curRead = bReaderFormat[i].bRead;

        if (type == bReaderFormat[i].id)
        {
            memcpy(
                TYPE<char*>(dst),
                fileData.data() + offset,
                curRead
            );
            std::cout << "offset: " << offset << std::endl;
            offset += curRead;
        }
    }
}

std::vector<char> OsrFile::GetString()
{
    char ReadSize;
    offset++;
    GetVal(&ReadSize, 'c');
    offset += ReadSize;
    std::cout << "str end offset: " << offset << std::endl;

    std::cout << "CUR " << std::hex << int(ReadSize) << std::endl;

    std::vector<char> str(ReadSize+1);
    
    memcpy(
        TYPE<char*>(str.data()), 
        TYPE<char*>(fileData.data() + offset),
        ReadSize
    );

    str[ReadSize] = '\0'; // \0 terminated

    return str;
}

void OsrFile::ReadStruct()
{
    try {
        GetVal(&sign.mode,      'c');
        GetVal(&sign.ver,       'i');
        std::cout << "version: " << sign.ver << std::endl;
        std::cout << "6062 | " << GetString().data() << std::endl;
        std::cout << "6062 | " << GetString().data() << std::endl;
        GetVal(&sign.r300,      's');
        GetVal(&sign.r100,      's');
        GetVal(&sign.r50,       's');
        GetVal(&sign.iCombos,   's');
        GetVal(&sign.niCombos,  's');
        GetVal(&sign.misses,    's');
        GetVal(&sign.points,    'i');
        GetVal(&sign.maxCombo,  's');
        GetVal(&sign.iiCombos,  'c');
        GetVal(&sign.modes,     'i');
        sign.hp = GetString().data();
        GetVal(&sign.time,      'l');
        GetVal(&sign.compData,  'i');
    } 
    catch(int& err) 
    {
        std::cout << err << std::endl;

        if (AttachConsole(ATTACH_PARENT_PROCESS))
        {
            FILE* stream;
            freopen_s(&stream, "CONOUT$", "w", stdout);
        }

        exit(err);
    }
}   

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

