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
    file.read(reinterpret_cast<char*>(tv.data()), fSize);

    fileData = tv;
}

int OsrFile::ReadSign()
{
    if (fileData.size() < fSize)
    {
        return NULL;
    }

    const char* rInstruct = "citttssssssiscicli"; // все до char* replayData;
    int rInstructSize = strlen(rInstruct);
    // строки чисто как указатели будут
    std::vector<int> toRead = AmountRead(rInstruct, fileData);

    int offset = 0;

    std::cout << rInstructSize << std::endl;

    for (int i = 0; i < rInstructSize; i++)
    {
        if (offset >= rInstructSize) return 1;

        if (toRead[i] == TYPE_STRING)
        {
            toRead[i] = (char)*(fileData.data()+offset+1);
            offset += 2;

            std::cout << toRead[i] << std::endl;

            for (int o = 0; o < toRead[i]; o++)
            {
                std::cout << fileData[offset+o];
            }

            std::cout << std::endl;
        }

        memcpy(&sign+offset, &fileData[offset], toRead[i]);
        offset += toRead[i];
    }

    return 0;
}

std::vector<int> AmountRead(const char* instruction, std::vector<char> data)
{
    if (sizeof(bReaderFormat) < 2) return {};

    std::vector<int> list;
    char ReaderStructSize = sizeof(bReaderFormat) / sizeof(bReaderFormat[0]);

    for (int i = 0; i < strlen(instruction); i++)
    {
        for (int j = 0; j < ReaderStructSize; j++)
        {
            if (instruction[i] == bReaderFormat[j].id)
            {
                list.push_back(bReaderFormat[j].bRead);
            }
        }
    }

    return list;
}
