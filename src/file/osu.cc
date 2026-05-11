/*
 * MIT License
 * Copyright (c) 2026 crelgd
 */

// МОЖЕТ НЕ ПРАВИЛЬНО ЧИТАТЬ ФАЙЛЫ ИЗ ПРОТИВОПОЛОЖНОЙ ОС

#include "file/osu.h"

namespace osu
{
    std::string OsuFile::ParserGetSection()
    {
        err = SkipVal(NEW_LINE, 2);

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

                statSection += fileData[fileOffset];
                fileOffset++;
            }

            if (fileData[fileOffset] == '[' || 
                    fileData[fileOffset] == ']')
            {
                fileOffset++;
            }
        }
        else if (fileData[fileOffset] != '[')
        {
            err = CFILE_OK;
            return "";
        }

        return Section = statSection;
    }

    // пусть будет так
    cFileErr OsuFile::SkipVal(const char* val, uint8_t bCount)
    {
        // бля ну ладна
        if (fileOffset >= fileData.size())
        {
            return CFILE_END;
        }

        if (memcmp(fileData.data() + fileOffset, val, bCount) != 0)
            return CFILE_SKIP_NOTH;

        while (memcmp(fileData.data() + fileOffset, val, bCount) == 0)
        {
            if (fileOffset >= fileData.size())
            {
                return CFILE_END;
            }

            fileOffset+=bCount;
        }

        if (memcmp(fileData.data() + fileOffset, val, bCount) == 0)
            fileOffset+=bCount;

        if (fileData[fileOffset] == '[') 
            return CFILE_NEW_SECTION;

        return CFILE_OK;
    }

    void OsuFile::ParserClearSection()
    {
        Section = "";
        err = CFILE_OK;
    }

    ConfOsuValue OsuFile::ParserGetValue()
    {
        ConfOsuValue val = {};
        int valIndex   = 0;

        std::vector<std::string> valArr(2);

        err = SkipVal(NEW_LINE, 2);
        if (err == CFILE_END || err == CFILE_NEW_SECTION)
            return {};

        while (fileOffset < fileData.size() && 
                fileData[fileOffset] != '\r')
        {
            if (valIndex > valArr.size()-1)
            {
                err = CFILE_ERR;
                return {};
            }

            if (fileData[fileOffset] == ':')
            {
                const char* valS = ":";
                err = SkipVal(valS, 1);

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

    cFileErr OsuFile::SkipComment()
    {
        if (fileData[fileOffset] == '/' && 
                fileData[fileOffset] == '/')
        {
            for (int i = 0; i < fileData.size() - fileOffset; i++)
            {
                err = SkipVal(NEW_LINE, 2);
                if (err != CFILE_SKIP_NOTH)
                    break;

                fileOffset++;
            }
        }

        return CFILE_OK;
    }

    std::vector<uint16_t> OsuFile::ParserGetComaSeparatedValue()
    {
        std::vector<uint16_t> valList;
        std::string valBfr = "";

        err = SkipVal(NEW_LINE, 2);
        if (err == CFILE_END || err == CFILE_NEW_SECTION)
            return {};

        while (fileOffset < fileData.size() && 
                fileData[fileOffset] != '\r')
        {
            SkipComment();

            if (fileData[fileOffset] == ',' || 
                    fileData[fileOffset+1] == '\r')
            {
                valList.push_back(std::stoi(valBfr));
                valBfr.clear();
            }

            valBfr += fileData[fileOffset];

            fileOffset++;
        }

        return valList;
    }

    ConfOsuValue OsuFile::ReadGeneralSection()
    {
        
    }

    cFileErr OsuFile::ReadStruct()
    {
        while (err != CFILE_END)
        {
            std::string sectionName = ParserGetSection();

            if (sectionName.size() == strlen("Events"))
            {
                //
            }
        }

        return CFILE_OK;
    }
}

