/*
 * MIT License
 * Copyright (c) 2026 crelgd
 */

// МОЖЕТ НЕ ПРАВИЛЬНО ЧИТАТЬ ФАЙЛЫ ИЗ ПРОТИВОПОЛОЖНОЙ ОС
//
// при новой строке в конце может выдавать ошибку какуюта

#pragma once

#include "base.h"

#ifdef _WIN32
const char NEW_LINE[2] = {'\r', '\n'};
#endif

#define OSU_CODE 0x80000000u
#define OSU_CODE_CHECK( _val ) ( ( _val & OSU_CODE ) != 0 )

#define OSU_WRITEVAL( _param_name, _struct, _out, _val )\
        if (_param_name == _struct.name)                \
            _out = _val;

#define OSU_UWRITEVAL( _param_name, _struct, _out, _val )   \
        if (_param_name == _struct.name) {                  \
            std::wstring u = base::ConvertWSTR(_val.c_str(), _val.length());\
            _out = u;}                                      \

typedef enum : uint32_t
{
    OSU_BEZIER     = 0xfffffff1,
    OSU_CENTIPETAL = 0xfffffff2,
    OSU_LINEAR     = 0xfffffff3,
    OSU_PCIRCLE    = 0xfffffff4
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
    std::string val; // потом
} ConfOsuValue;

typedef struct 
{
    std::string AudioFileName;
    int AudioLeadIn;
    std::string AudioHash;
    int PreviewTime;
    int Countdown;
    std::string SampleSet;
    float StackLeniency;
    int Mode;
    bool LetterboxInBreaks;
    bool StoryFileInFront;
    bool UseSkinSprites;
    bool AlwaysShowPlayfield;
    std::string OverlayPosition;
    std::string SkinPreference;
    bool EpilepcyWarning;
    int CountdownOffset;
    bool SpecialStyle;
    bool WidescreenStoryboard;
} OsuGlobalSection;

typedef struct
{
    std::string Title;
    std::wstring TitleUnicode;
    std::string Artist;
    std::wstring ArtistUnicode;
    std::string Creator;
    std::string Version;
    std::string Source;
    std::vector<std::string> Tags; // разделенные пробелом
    int BeatmapID;
    int BeatmapSetID;
} OsuMetadataSection;

namespace osu
{
    // где возращаемое значение имеет тип FileErr
    // то не читать GetError()
    class OsuFile : public base::File
    {
    public:
        cFileErr ReadGeneralSection(OsuGlobalSection &glsec);
        cFileErr ReadMetadataSection(OsuMetadataSection &metasec);
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
        cFileErr SkipVal(const char *val, uint8_t bCount);
        cFileErr ParserMetadataTagsConvert(std::string TagsBfr, OsuMetadataSection &oms);
        
    private:
        std::string Section;
    };
}

// туду сделать массив с указателем на секции
