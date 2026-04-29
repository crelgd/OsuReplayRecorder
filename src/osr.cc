/*
 * MIT License
 * Copyright (c) 2026 crelgd
 */

#include "osr.h"

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
char* OsrFile::GetVal(char type)
{
    char bReaderSize = sizeof(bReaderFormat) / sizeof(bReaderFormat[0]);

    for (int i = 0; i < bReaderSize; i++)
    {
        char curRead = bReaderFormat[i].bRead;

        if (type == bReaderFormat[i].id)
        {
            std::vector<char> arr(curRead);
            memcpy(
                TYPE<char*>(arr.data()),
                fileData.data() + offset,
                curRead
            );
            offset += curRead;
            
            return arr.data();
        }
    }

    return NULL;
}

std::vector<char> OsrFile::GetString()
{
    char ReadSize;
    offset++; ReadSize = TYPE<char>(GetVal('c'));

    std::vector<char> str(ReadSize+1);
    
    if (memcpy(
        TYPE<char*>(str.data()), 
        TYPE<char*>(fileData.data() + offset),
        ReadSize
    ) != str.data()) 
    {
        return {};
    }

    str[ReadSize] = '\0'; // \0 terminated

    return str;
}

void OsrFile::ReadStruct()
{
    sign.mode = TYPE<char>(GetVal('c'));
    sign.ver = TYPE<int>(GetVal('i'));
    sign.m5card = TYPE<char*>(GetString().data());
    sign.player = TYPE<char*>(GetString().data());
    sign.m5replay = TYPE<char*>(GetString().data());
    sign.r300 = TYPE<short>(GetVal('s'));
    sign.r100 = TYPE<short>(GetVal('s'));
    sign.r50 = TYPE<short>(GetVal('s'));
    sign.iCombos = TYPE<short>(GetVal('s'));
    sign.niCombos = TYPE<short>(GetVal('s'));
    sign.misses = TYPE<short>(GetVal('s'));
    sign.points = TYPE<int>(GetVal('i'));
    sign.maxCombo = TYPE<short>(GetVal('s'));
    sign.iiCombos = TYPE<char>(GetVal('c'));
    sign.modes = TYPE<int>(GetVal('i'));
    sign.hp = TYPE<char*>(GetString().data());
    sign.time = TYPE<long>(GetVal('l'));
    sign.compData = TYPE<int>(GetVal('i'));
}