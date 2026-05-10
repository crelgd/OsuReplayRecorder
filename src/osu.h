/*
 * MIT License
 * Copyright (c) 2026 crelgd
 */

// МОЖЕТ НЕ ПРАВИЛЬНО ЧИТАТЬ ФАЙЛЫ ИЗ ПРОТИВОПОЛОЖНОЙ ОС

#pragma once

#include "base.h"

typedef struct 
{
    std::string name;
    std::string val;
} ConfOsuValue;

namespace osu
{
    // где возращаемое значение имеет тип FileErr
    // то не читать GetError()
    class OsuFile : public base::File
    {
    public:
        cFileErr ReadGeneral();
        cFileErr ReadStruct();
        // GetError возвращает 
        // - CFILE_OK если курсор не на секции
        // - CFILE_ERR если курсор выходит за файл
        // - CFILE_END есликонец файла (можно проверять без пустой строки)
        std::string ParserGetSection();
        void ParserClearSection();
        // GetError возвращает
        ConfOsuValue ParserGetValue();

    private:
        // единственная функция где используется FILE_END
        // показывает что файл закончился и возвращает курсор
        // в место откуда была она вызвана
        // возвращает
        // - CFILE_END
        // - CFILE_NEW_SECTION
        // - CFILE_OK
        cFileErr SkipVal(const char val);
    private:
        std::string Section;
    };
}
