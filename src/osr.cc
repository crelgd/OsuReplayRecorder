/*
 * MIT License
 * Copyright (c) 2026 crelgd
 */

#include "osr.h"

#include <windows.h>
#include <fstream>
#include <ios>

OsrFile::OsrFile(std::string fName)
{
    std::ifstream file(fName, std::ios::in | std::ios::binary);

    file.seekg(0, std::ios::end);
    fSize = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<uint8_t> tv(fSize);
    file.read(TYPE<char*>(tv.data()), fSize);

    fileData = tv;
}

int OsrFile::Read()
{
    if (fileData.size() < fSize)
    {
        return 1;
    }

    sign.modInfo = NULL;
    ReadStruct();

    if (PlayDecode() != 0)
    {
        std::cout << "lzma read err" << std::endl;
        return 1;
    }

    return 0;
}

std::string OsrFile::GetString(int lebSize)
{
    uint8_t byte = GetVal<uint8_t>();
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
            str.resize(strSize);
        
            memcpy(&str[0], fileData.data() + offset, strSize);
            offset += strSize;
        }
        break;
    }

    return str;
}

void OsrFile::ReadStruct()
{
    sign.mode       = GetVal<uint8_t>();
    sign.ver        = GetVal<uint32_t>();
    sign.md5card    = GetString(1);
    sign.player     = GetString(1);
    sign.md5replay  = GetString(1);
    sign.r300       = GetVal<uint16_t>();
    sign.r100       = GetVal<uint16_t>();
    sign.r50        = GetVal<uint16_t>();
    sign.iCombos    = GetVal<uint16_t>();
    sign.niCombos   = GetVal<uint16_t>();
    sign.misses     = GetVal<uint16_t>();
    sign.points     = GetVal<uint32_t>();
    sign.maxCombo   = GetVal<uint16_t>();
    sign.iiCombos   = GetVal<uint8_t>();
    sign.modes      = GetVal<uint32_t>();
    sign.hp         = GetString(1);
    sign.time       = GetVal<uint64_t>();
    sign.compData   = GetVal<uint32_t>();

    compDataOffset = offset; 
    offset = compDataOffset + sign.compData;

    sign.resId      = GetVal<uint64_t>();

    if (sign.modes == (1 << 23))
    {
        sign.modInfo= GetVal<double>();
    }
}   

int OsrFile::uleb128_decode()
{
    int out = 0;
    int shift = 0;
    uint8_t byte;

    do
    {
        byte =  GetVal<uint8_t>();
        uint8_t byteDecompile = byte & 0x7f;
        out |= byteDecompile << shift;

        shift += 7; 
    } while((byte & 0x80) != 0);

    return out;
}
std::vector<uint8_t> OsrFile::uleb128_encode(int val)
{
    int shift = 0;
    std::vector<uint8_t> arr;

    while (val != 0)
    {
        uint8_t byte = val & 0x7f;
        val >>= 7;

        if (val != 0)
        {
            byte |= 0x80;
        }

        arr.push_back(uint8_t(byte));
    }

    return arr;
}

int OsrFile::PlayDecode()
{
//  // содержит выходные данные свойств
//  std::vector<lzma_options_lzma> opt(3);
//  std::vector<uint8_t> propByte;

//   // получение 3 байтов properties
//   for (int i = 0; i < 3; i++)
//       propByte.push_back(GetVal<uint8_t>());

//   if (lzmaPropertiesDecode(opt, propByte))
//       return 1;

//   uint32_t dict = GetVal<uint32_t>();
//   uint64_t rDict = GetVal<uint64_t>();

    lzma_ret errcode;

    stream = new lzma_stream();
    if (!stream)
        return 1;

    *stream = LZMA_STREAM_INIT;

    errcode = lzma_auto_decoder(stream, UINT64_MAX, LZMA_IGNORE_CHECK);
    if (errcode != LZMA_OK) 
        return 1;

    std::vector<uint8_t> out(8024);

    stream->next_in = fileData.data() + compDataOffset;
    stream->avail_in = sign.compData;

    while (errcode != LZMA_STREAM_END) 
    {   
        stream->next_out = out.data();
        stream->avail_out = out.size();

        // декод
        errcode = lzma_code(stream, LZMA_RUN);
        if (errcode != LZMA_OK && errcode != LZMA_STREAM_END)
            return 1;

        if (errcode == LZMA_STREAM_END)
            break;

        // for (int i = 0; i < out.size() - stream->avail_out; i++)
        // {
        //     std::cout << out[i];
        // }
        // std::cout << std::endl;
    }   

    return 0;
}

OsrFile::~OsrFile()
{
    if (stream)
    {
        uint64_t progIn, progOut;
        lzma_end(stream);
        delete stream;
    }
}

// int OsrFile::lzmaPropertiesDecode(std::vector<lzma_options_lzma>& lzma_opt, 
//         std::vector<uint8_t>& prop3)
// {
//     if (prop3.size() != 3)
//         return 1;

//     if (lzma_opt.size() != 3)
//         return 1;

//     for (int i = 0; i < 3; i++)
//     {
//         if (prop3[i] > (4 * 5 + 4) * 9 + 8)
//             return 1;

//         lzma_opt[i].pb = prop3[i] / (9 * 5);
//         prop3[i] -= lzma_opt[i].pb * 9 * 5;
//         lzma_opt[i].lp = prop3[i] / 9;
//         lzma_opt[i].lc = prop3[i] - lzma_opt[i].lp * 9;
//     }

//     return 0;
// }

