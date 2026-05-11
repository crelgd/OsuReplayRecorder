/*
 * MIT License
 * Copyright (c) 2026 crelgd
 */

#include "file/osr.h"

#include <fstream>
#include <ios>


namespace osr
{
    // не буду убирать лень
    OsrErr OsrFile::Read()
    {
        if (fileData.size() < fileSize)
        {
            return OSR_ERR;
        }

        sign.modInfo = NULL;
        ReadStruct();

        return OSR_OK;
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
            
                fileOffset += strSize;
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

        compDataOffset = fileOffset; 
        fileOffset = compDataOffset + sign.compData;

        sign.resId      = GetVal<uint64_t>();

        if (sign.modes == (1 << 23))
        {
            sign.modInfo= GetVal<double>();
            spdlog::info("Был загружен modInfo");
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

    OsrErr OsrFile::DecodeInit()
    {
        lzma_ret errcode;

        stream = new lzma_stream();
        if (!stream)
            return OSR_ERR;

        *stream = LZMA_STREAM_INIT;

        errcode = lzma_auto_decoder(stream, UINT64_MAX, LZMA_IGNORE_CHECK);
        if (errcode != LZMA_OK) 
        {
            spdlog::error("Поток LZMA не был инициализирован");
            return OSR_ERR;
        }

        stream->next_in = fileData.data() + compDataOffset;
        stream->avail_in = sign.compData;

        return OSR_OK;
    }

    OsrErr OsrFile::Decode(std::vector<uint8_t>& bfr, size_t& written)
    {
        lzma_ret errcode;

        stream->next_out = bfr.data();
        stream->avail_out = bfr.size();

        // декод
        errcode = lzma_code(stream, LZMA_RUN);
        written = bfr.size() - stream->avail_out;

        if (errcode != LZMA_OK && errcode != LZMA_STREAM_END)
        {
            spdlog::error("При декодировании произошла ошибка");
            return OSR_ERR;
        }

        if (errcode == LZMA_STREAM_END)
            return OSR_DECODE_END;

        return OSR_OK;
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

