/*
 * MIT License
 * Copyright (c) 2026 crelgd
 */

// МОЖЕТ НЕ ПРАВИЛЬНО ЧИТАТЬ ФАЙЛЫ ИЗ ПРОТИВОПОЛОЖНОЙ ОС

// при новой строке в конце может выдавать ошибку какуюта

#pragma once

#include "base.h"

typedef struct 
{
    std::string name;
    std::string val;
} ConfOsuValue;

#ifdef _WIN32
const char NEW_LINE[2] = {'\r', '\n'};
#endif

namespace osu
{
    // где возращаемое значение имеет тип FileErr
    // то не читать GetError()
    class OsuFile : public base::File
    {
    public:
        cFileErr ReadGeneralSection();
        cFileErr ReadStruct();
        // GetError возвращает 
        // - CFILE_OK если курсор не на секции
        // уточннение если курсор не на начале секкции '['
        // - CFILE_ERR если курсор выходит за файл
        // - CFILE_END есликонец файла (можно проверять без пустой строки)
        std::string ParserGetSection();
        void ParserClearSection();
        // GetError возвращает
        ConfOsuValue ParserGetValue();
        std::vector<uint16_t> ParserGetComaSeparatedValue();
        // вызывать в цикле с проверкой на конец файла
        cFileErr SkipComment();

    private:
        // единственная функция где используется FILE_END
        // показывает что файл закончился и возвращает курсор
        // в место откуда была она вызвана
        // возвращает
        // - CFILE_END
        // - CFILE_NEW_SECTION
        // - CFILE_OK
        cFileErr SkipVal(const char* val, uint8_t bCount);
    private:
        std::string Section;
    };
}
