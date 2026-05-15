/*
 * MIT License
 * Copyright (c) 2026 crelgd
 */

// МОЖЕТ НЕ ПРАВИЛЬНО ЧИТАТЬ ФАЙЛЫ ИЗ ПРОТИВОПОЛОЖНОЙ ОС
//
// при новой строке в конце может выдавать ошибку какуюта
//
// в этом файле писать макросы в верхнем индексе
// и после переменные в скобках делать посередине ( ну типо )
// комы писать как угодно, главное чтоб понятно было
// комментировать только проблемные функции
// на стиль кода поуй

#pragma once

#include "base.h"

#ifdef _WIN32
const char NEW_LINE[2] = {'\r', '\n'};
#endif

#define OSU_CODE 0x80000000u
#define OSU_CODE_CHECK( _val ) ( ( _val & OSU_CODE ) != 0 )

typedef enum
{
    OSU_BEZIER     = 0xfffffff1u,
    OSU_CENTIPETAL = 0xfffffff2u,
    OSU_LINEAR     = 0xfffffff3u,
    OSU_PCIRCLE    = 0xfffffff4u
} OsuCodeEnum;

typedef struct 
{
    char type;
    uint32_t code;
} OsuSliderParse;

const OsuSliderParse OsuSliderCode[] = {
    {'B', OSU_BEZIER        },
    {'C', OSU_CENTIPETAL    },
    {'L', OSU_LINEAR        },
    {'P', OSU_PCIRCLE       }
};

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
        // cFileErr ReadGeneralSection();
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
        // если старший байт имеет еденичку в конце, означает конец
        // строки дляслайдеров или другого подумаю
        // крч пример 
        // OSU_BEZIER начало
        // OSU_CODE_CHECK( _val ) конец
        std::vector<int64_t> ParserGetComaSeparatedValue();
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
