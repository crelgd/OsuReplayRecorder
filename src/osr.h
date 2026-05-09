/*
 * MIT License
 * Copyright (c) 2026 crelgd
 */

#pragma once

#include "lzma/lzma.h"

#define TYPE    reinterpret_cast

// #define TYPE_ARRAY      0x7201
// #define TYPE_STRING     0x7200

typedef enum
{
    OSR_ERR,
    OSR_OK,
    OSR_DECODE_END
} OsrErr;

typedef struct
{
    uint8_t mode;      // (0 = osu!, 1 = osu!taiko, 2 = osu!catch, 3 = osu!mania)
    uint32_t ver;
    std::string md5card;
    std::string player;
    std::string md5replay;
    uint16_t r300;          // Число попаданий в "300"
    uint16_t r100;          // Число попаданий в "100" в osu!, "150" в osu!taiko, "100" в osu!catch, "100" в osu!mania
    uint16_t r50;           // Число попаданий в "50" в osu!, "50" в osu!mania, или маленьких фруктов в osu!catch
    uint16_t iCombos;       // Число идеальных комбо (Geki) в osu! или "MAX" в osu!mania
    uint16_t niCombos;      // Число неидеальных комбо (Katu) в osu! или "200" в osu!mania
    uint16_t misses;        // Число промахов
    uint32_t points;        // Суммарное число полученных очков  
    uint16_t maxCombo;      // Самое большое набранное комбо
    uint8_t iiCombos;       // Идеальное комбо (1 = без промахов, слайдербрейков и пропущенных концов слайдеров)
    uint32_t modes;         // Используемые моды
    std::string hp;         // Уровень здоровья: разделенные запятыми пары u/v, 
                            // где u — это время в миллисекундах в песне, 
                            // а v — значение с плавающей запятой от 0 до 1, 
                            // равное количеству здоровья в данный момент времени 
                            // (0 = полоска здоровья пуста, 1 = полоска здоровья заполнена)    
    uint64_t time;          // Отметка времени
    uint32_t compData;      // 	Длина сжатых данных реплея (в байтах)
//  uint8_t* replayData;    // Сжатые данные реплея
    uint64_t resId;         // 	Онлайн-идентификатор результата
    double modInfo;         // Дополнительная информация о модах. Присутствует только при включенном моде Target Practice
} OsrSign;

namespace osr
{
    typedef struct {
        int w, x, y, z;
    } Decompile;

    class OsrFile
    {
    public:
        ~OsrFile();

        void load(std::string fName);

        OsrErr Read();

        OsrErr DecodeInit();
        OsrErr Decode(std::vector<uint8_t>& bfr);
    private:

        template <typename T>
        T GetVal()
        {
            T val;
            memcpy(&val, fileData.data() + offset, sizeof(T));
            offset += sizeof(T);

            return val;
        }

        std::string GetString(int lebSize);
        void ReadStruct();

        int uleb128_decode();
        std::vector<uint8_t> uleb128_encode(int val);

//   int lzmaPropertiesDecode(std::vector<lzma_options_lzma>& lzma_opt, 
//          std::vector<uint8_t>& prop3);

    public:
        OsrSign sign;
    private:
        size_t fSize;
        uint64_t offset = 0;
        uint64_t compDataOffset = 0;
        std::vector<uint8_t> fileData;
        lzma_stream* stream;
    };

    // читает весь буффер и записывает значение в виде массива из структур
    std::vector<Decompile> ReadDecompile(std::vector<uint8_t>& bfr);
}

// class OSRException
// {
// public:
//     OSRException(int errd)
//     {
//         err = errd;
//     }
    
//     virtual int what() throw()
//     {
//         return err;
//     }

// public:
//     int err;
// };

// std::vector<int> AmountRead(const char* instruction, std::vector<char> data);
