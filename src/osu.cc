/*
 * MIT License
 * Copyright (c) 2026 crelgd
 */

// МОЖЕТ НЕ ПРАВИЛЬНО ЧИТАТЬ ФАЙЛЫ ИЗ ПРОТИВОПОЛОЖНОЙ ОС

#include "osu.h"

namespace osu
{
    std::string OsuFile::ParserGetSection()
    {
        err = SkipVal('\n');

        if (Section.size() != 0)
            return Section;

        std::string statSection = "";

        if (fileData[fileOffset] == '[')
        {
            fileOffset++;
            while (fileData[fileOffset] != ']' && 
                fileData[fileOffset] != '[')
            {
                if (fileOffset >= fileData.size())
                {
                    err = CFILE_ERR;
                    return "";
                }

                statSection += fileData[fileOffset++];
            }
        }
        else if (fileData[fileOffset] != '[')
        {
            err = CFILE_OK;
            return "";
        }

        return Section = statSection;
    }

    cFileErr OsuFile::SkipVal(const char val)
    {
        uint64_t statOffset = fileOffset;

        while (fileData[fileOffset++] == val)
        {
            if (fileOffset >= fileData.size())
            {
                fileOffset = statOffset;
                return CFILE_END;
            }
        }

        if (fileData[fileOffset] == '[') 
            return CFILE_NEW_SECTION;

        return CFILE_OK;
    }

    void OsuFile::ParserClearSection()
    {
        Section = "";
    }

    ConfOsuValue OsuFile::ParserGetValue()
    {
        ConfOsuValue val = {};
        int valIndex   = 0;

        std::vector<std::string> valArr(2);

        err = SkipVal('\n');
        if (err == CFILE_END || err == CFILE_NEW_SECTION)
            return {};

        while (fileOffset < fileData.size() && 
            fileData[fileOffset] != '\n')
        {
            if (valIndex > valArr.size()-1)
            {
                err = CFILE_ERR;
                return {};
            }

            if (fileData[fileOffset] == ':')
            {
                err = SkipVal(':');
                valIndex++;

                if (err != CFILE_OK)
                    return {};
            }

            valArr[valIndex] += fileData[fileOffset];

            fileOffset++;
        }

        val.name = valArr[0];
        val.val = valArr[1];

        return val;
    }

    cFileErr OsuFile::ReadGeneral()
    {
        return CFILE_OK;
    }

    cFileErr OsuFile::ReadStruct()
    {
        return CFILE_OK;
    }
}

