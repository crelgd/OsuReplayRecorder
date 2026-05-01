/*
 * MIT License
 * Copyright (c) 2026 crelgd
 */

#pragma once

#define TYPE    reinterpret_cast

typedef struct
{
    char id;
    char bRead;
} bReader;

// #define TYPE_ARRAY      0x7201
// #define TYPE_STRING     0x7200

// чисто чтоб не запутатся написал
// забейте
const bReader bReaderFormat[] = {
    {'i', 4}, // int
    {'c', 1}, // char
    {'s', 2}, // short
    {'l', 8}, // long
    {'d', 1}, // double
};

typedef struct
{
    char mode;      // (0 = osu!, 1 = osu!taiko, 2 = osu!catch, 3 = osu!mania)
    int ver;
    std::string md5card;
    std::string player;
    std::string md5replay;
    short r300;     // Число попаданий в "300"
    short r100;     // Число попаданий в "100" в osu!, "150" в osu!taiko, "100" в osu!catch, "100" в osu!mania
    short r50;      // Число попаданий в "50" в osu!, "50" в osu!mania, или маленьких фруктов в osu!catch
    short iCombos;  // Число идеальных комбо (Geki) в osu! или "MAX" в osu!mania
    short niCombos; // Число неидеальных комбо (Katu) в osu! или "200" в osu!mania
    short misses;   // Число промахов
    int points;     // Суммарное число полученных очков  
    short maxCombo; // Самое большое набранное комбо
    char iiCombos;  // Идеальное комбо (1 = без промахов, слайдербрейков и пропущенных концов слайдеров)
    int modes;      // Используемые моды
    std::string hp;       // Уровень здоровья: разделенные запятыми пары u/v, 
                    // где u — это время в миллисекундах в песне, 
                    // а v — значение с плавающей запятой от 0 до 1, 
                    // равное количеству здоровья в данный момент времени 
                    // (0 = полоска здоровья пуста, 1 = полоска здоровья заполнена)    
    long time;      // Отметка времени
    int compData;   // 	Длина сжатых данных реплея (в байтах)
    char* replayData; // Сжатые данные реплея
    long resId;     // 	Онлайн-идентификатор результата
    double modInfo; // Дополнительная информация о модах. Присутствует только при включенном моде Target Practice
} OsrSign;

class OsrFile
{
public:
    OsrFile(std::string fName);
    int ReadSign(); // читает до replayData
private:
    void GetVal(void* dst, char type);
    std::vector<char>GetString(int lebSize);
    void ReadStruct();

public:
    OsrSign sign;
private:
    size_t fSize;
    int offset = 0;
    std::vector<char> fileData;
};

class OSRException
{
public:
    OSRException(int errd)
    {
        err = errd;
    }
    
    virtual int what() throw()
    {
        return err;
    }

public:
    int err;
};

int uleb128_decode(std::vector<unsigned char>& bytes);
std::vector<char> uleb128_encode(int val);

// std::vector<int> AmountRead(const char* instruction, std::vector<char> data);
