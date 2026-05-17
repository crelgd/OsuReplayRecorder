/*
 * MIT License
 * Copyright (c) 2026 crelgd
 */

// МОЖЕТ НЕ ПРАВИЛЬНО ЧИТАТЬ ФАЙЛЫ ИЗ ПРОТИВОПОЛОЖНОЙ ОС

#include "file/osu.h"
#include <iomanip>
#include <ios>

namespace osu
{
    std::string OsuFile::ParserGetSection()
    {
        err = SkipVal(NEW_LINE, 2);

        if (err == CFILE_END) 
            return Section;

        if (!Section.empty())
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
            err = CFILE_NOT_SECTION;
            return "";
        }

        return Section = statSection;
    }

    // пусть будет так
    cFileErr OsuFile::SkipVal(const char* val, uint8_t bCount)
    {
        // бля ну ладна
        if (fileOffset >= fileSize)
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

        // чтоб не вылетало, вообще хз как сделать
        if (fileOffset >= fileSize)
        {
            return CFILE_END;
        }

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

        while (fileOffset < fileSize && 
                fileData[fileOffset] != '\r') // не менять на \n, ломает скип пробелов
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
                err = SkipVal(" ", 1);

                valIndex++;

                if (err != CFILE_OK && err != CFILE_SKIP_NOTH)
                    return {};
            }

            valArr[valIndex] += fileData[fileOffset];

            fileOffset++;
        }

        val.name = valArr[0];
        val.val = valArr[1];

        // auto flags = std::cout.flags();
        // auto fill  = std::cout.fill();

        LOG("\tParserGetValue() name=" << val.name <<
            "\n\tval=" << val.val);

        // for (int i = 0; i < valArr[1].size(); i++)
        //     LOG("\t\tParserGetValue() val in hex=" <<  std::hex << std::setfill('0') << std::setw(2)<< std::uppercase << (int)(unsigned char)valArr[1][i]);

        // std::cout.flags(flags);
        // std::cout.fill(fill);

        return val;
    }

    cFileErr OsuFile::SkipComment()
    {
        if (fileData[fileOffset] == '/' && 
                fileData[fileOffset+1] == '/')
        {
            while (fileOffset < fileSize)
            {
                err = SkipVal(NEW_LINE, 2);
                if (err != CFILE_SKIP_NOTH)
                    break;

                fileOffset++;
            }
        }

        return CFILE_OK;
    }

    // похуй на код кароче
    std::vector<int64_t> OsuFile::ParserGetComaSeparatedValue()
    {
        std::vector<int64_t> valList;
        std::string valBfr = "";
        bool nstdType = false;

        err = SkipVal(NEW_LINE, 2);
        if (err == CFILE_END || err == CFILE_NEW_SECTION)
            return {};

        while (fileOffset < fileSize && 
                fileData[fileOffset] != '\n')
        {
            SkipComment();

            if (fileData[fileOffset] == ',')
            {
                if (nstdType) valList.push_back(std::stoul(valBfr) | OSU_CODE);
                else valList.push_back(std::stoul(valBfr));

                nstdType = false;

                valBfr.clear();
            }
            else if (fileData[fileOffset] == ':' || fileData[fileOffset] == '|')
            {
                valList.push_back(std::stoi(valBfr));
                valBfr.clear();

                nstdType = true;
            }
            else // как я недодумался
            {
                for (int i = 0; i < 4; i++)
                {
                    if (fileData[fileOffset] == OsuSliderCode[i].type)
                    {
                        valList.push_back(static_cast<int64_t>(OsuSliderCode[i].code));
                        fileOffset++; SkipVal("|", 1);
                        continue;
                    }
                }

                valBfr += fileData[fileOffset];
            }

            if (fileOffset < fileSize)
                    fileOffset++;
        }

        if (!valBfr.empty())
        {
            valList.push_back(std::stoi(valBfr));
        }

        return valList;
    }

    cFileErr OsuFile::ReadGeneralSection(OsuGlobalSection &glsec)
    {
        cFileErr skip = SkipVal(NEW_LINE, 2);

        if (skip == CFILE_ERR)
            return CFILE_ERR;

        err = CFILE_OK;
        glsec = {};

        if (SettingUpSection("General"))
        {
            while (err != CFILE_NEW_SECTION)
            {
                if (err == CFILE_END || err == CFILE_ERR)
                    return err;

                ConfOsuValue cov = ParserGetValue();
                if (err == CFILE_ERR) // 
                    return err;

                OSU_WRITESTR  ("AudioFilename"            , cov, glsec.AudioFileName          , cov.val);
                OSU_WRITEINT  ("AudioLeadIn"              , cov, glsec.AudioLeadIn            , cov.val);
                OSU_WRITESTR  ("AudioHash"                , cov, glsec.AudioHash              , cov.val);
                OSU_WRITEINT  ("PreviewTime"              , cov, glsec.PreviewTime            , cov.val);
                OSU_WRITEINT  ("Countdown"                , cov, glsec.Countdown              , cov.val);
                OSU_WRITESTR  ("SampleSet"                , cov, glsec.SampleSet              , cov.val);
                OSU_WRITEFLOAT("StackLeniency"            , cov, glsec.StackLeniency          , cov.val);
                OSU_WRITEINT  ("Mode"                     , cov, glsec.Mode                   , cov.val);
                OSU_WRITEBOOL ("LetterboxInBreaks"        , cov, glsec.LetterboxInBreaks      , cov.val);
                OSU_WRITEBOOL ("StoryFileInFront"         , cov, glsec.StoryFileInFront       , cov.val);
                OSU_WRITEBOOL ("UseSkinSprites"           , cov, glsec.UseSkinSprites         , cov.val);
                OSU_WRITEBOOL ("AlwaysShowPlayfield"      , cov, glsec.AlwaysShowPlayfield    , cov.val);
                OSU_WRITESTR  ("OverlayPosition"          , cov, glsec.OverlayPosition        , cov.val);
                OSU_WRITESTR  ("SkinPreference"           , cov, glsec.SkinPreference         , cov.val);
                OSU_WRITEBOOL ("EpilepcyWarning"          , cov, glsec.EpilepcyWarning        , cov.val);
                OSU_WRITEINT  ("CountdownOffset"          , cov, glsec.CountdownOffset        , cov.val);
                OSU_WRITEBOOL ("SpecialStyle"             , cov, glsec.SpecialStyle           , cov.val);
                OSU_WRITEBOOL ("WidescreenStoryboard"     , cov, glsec.WidescreenStoryboard   , cov.val);
            }
        } else return CFILE_ERR;

        return CFILE_OK;
    }

    cFileErr OsuFile::ReadMetadataSection(OsuMetadataSection &metasec)
    {
        cFileErr skip = SkipVal(NEW_LINE, 2);

        if (skip == CFILE_ERR)
        {
            LOG("ERR: ReadMetadataSection() skip == CFILE_ERR");
            return CFILE_ERR;
        }

        err = CFILE_OK;
        metasec = {};
        
        std::string TagsBfr;

        if (SettingUpSection("Metadata"))
        {
            while (err != CFILE_NEW_SECTION)
            {
                if (err == CFILE_END || err == CFILE_ERR)
                {
                    LOG("ERR: ReadMetadataSection() err == CFILE_END || err == CFILE_ERR");
                    return err;
                }

                ConfOsuValue cov = ParserGetValue();
                if (err == CFILE_ERR) // 
                {
                    LOG("ERR: ReadMetadataSection() err == CFILE_ERR");
                    return err;
                }
             
                OSU_WRITESTR  ("Title"         , cov, metasec.Title        , cov.val);
                OSU_UWRITESTR ("TitleUnicode"  , cov, metasec.TitleUnicode , cov.val);
                OSU_WRITESTR  ("Artist"        , cov, metasec.Artist       , cov.val);
                OSU_UWRITESTR ("ArtistUnicode" , cov, metasec.ArtistUnicode, cov.val);
                OSU_WRITESTR  ("Creator"       , cov, metasec.Creator      , cov.val);
                OSU_WRITESTR  ("Version"       , cov, metasec.Version      , cov.val);
                OSU_WRITESTR  ("Source"        , cov, metasec.Source       , cov.val);
                OSU_WRITESTR  ("Tags"          , cov, TagsBfr              , cov.val);
                OSU_WRITEINT  ("BeatmapID"     , cov, metasec.BeatmapID    , cov.val);
                OSU_WRITEINT  ("BeatmapSetID"  , cov, metasec.BeatmapSetID , cov.val);
            }
        } else return CFILE_ERR;

        if (ParserMetadataTagsConvert(TagsBfr, metasec) != CFILE_OK)
        {
            LOG("ERR: ParserMetadataTagsConvert() != CFILE_OK");
            return CFILE_ERR;
        }

        return CFILE_OK;
    }

    cFileErr OsuFile::ParserMetadataTagsConvert(std::string TagsBfr, OsuMetadataSection &oms)
    {   
        if (TagsBfr.empty())
            return CFILE_ERR;

        std::string bfr;

        for (int i = 0; i < TagsBfr.length(); i++)
        {
            if (TagsBfr[i] == ' ')
            {
                oms.Tags.push_back(bfr);
                bfr.clear();
            } else
            {
                bfr += TagsBfr[i];
            }
        }

        if (!bfr.empty())
            oms.Tags.push_back(bfr);

        return CFILE_OK;
    }

    void OsuFile::GetAllSections()
    {
        err = CFILE_OK;

        while (err != CFILE_END)
        {
            if (!Section.empty())
                ParserClearSection();
            
            ParserGetSection();

            if (err == CFILE_NOT_SECTION)
            {
                for (int i = 0; i < (fileSize - fileOffset); i++)
                {
                    if (fileOffset >= fileSize)
                        return;

                    if (fileData[fileOffset] == '[')
                        break;

                    fileOffset++;
                }

                ParserGetSection();
                if (err == CFILE_NOT_SECTION)
                    return;
            }

            LOG("GetAllSections() writed: " << Section);

            SectionTable nst;
            nst.section = Section;
            nst.offset = fileOffset;

            sectionTable.push_back(nst);
        }
    }

    cFileErr OsuFile::ReadDifficultSection(OsuDifficultSection& diffsec)
    {
        cFileErr skip = SkipVal(NEW_LINE, 2);

        if (skip == CFILE_ERR)
        {
            LOG("ERR: ReadDifficultSection() skip == CFILE_ERR");
            return CFILE_ERR;
        }

        err = CFILE_OK;
        diffsec = {};

        if (SettingUpSection("Difficulty"))
        {
            while (err != CFILE_NEW_SECTION)
            {
                if (err == CFILE_END || err == CFILE_ERR)
                {
                    LOG("ERR: ReadDifficultSection() err == CFILE_END || err == CFILE_ERR");
                    return err;
                }

                ConfOsuValue cov = ParserGetValue();
                if (err == CFILE_ERR) // 
                {
                    LOG("ERR: ReadDifficultSection() err == CFILE_ERR");
                    return err;
                }

                OSU_WRITEFLOAT("HPDrainRate"      , cov, diffsec.HPDrainRate      , cov.val);
                OSU_WRITEFLOAT("CircleSize"       , cov, diffsec.CircleSize       , cov.val);
                OSU_WRITEFLOAT("OverallDifficulty", cov, diffsec.OverallDifficulty, cov.val);
                OSU_WRITEFLOAT("ApproachRate"     , cov, diffsec.ApproachRate     , cov.val);
                OSU_WRITEFLOAT("SliderMultiplier" , cov, diffsec.SliderMultiplier , cov.val);
                OSU_WRITEFLOAT("SliderTickRate"   , cov, diffsec.SliderTickRate   , cov.val);
            }
        } else return CFILE_ERR;

        return CFILE_OK;
    }

    bool OsuFile::SettingUpSection(std::string section)
    {
        for (int i = 0; i < sectionTable.size(); i++)
        {
            if (sectionTable[i].section.compare(section) == 0)
            {
                fileOffset = sectionTable[i].offset;
                return true;
            }
        }

        return false;
    }

    cFileErr OsuFile::ReadColoursSection(OsuColoursSection& colsec)
    {
        cFileErr skip = SkipVal(NEW_LINE, 2);

        if (skip == CFILE_ERR)
        {
            LOG("ERR: ReadColoursSection() skip == CFILE_ERR");
            return CFILE_ERR;
        }

        err = CFILE_OK;
        colsec = {};

        std::string sCombo;
        std::string sSliderTrackOverride;
        std::string sSliderBorder;

        if (SettingUpSection("Colours"))
        {
            while (err != CFILE_NEW_SECTION)
            {
                if (err == CFILE_END || err == CFILE_ERR)
                {
                    LOG("ERR: ReadColoursSection() err == CFILE_END || err == CFILE_ERR");
                    return err;
                }

                ConfOsuValue cov = ParserGetValue();
                if (err == CFILE_ERR) // 
                {
                    LOG("ERR: ReadColoursSection() err == CFILE_ERR");
                    return err;
                }

                if (cov.name.compare(0, 5, "Combo") == 0)
                {
                    sCombo += cov.val + ",";
                    LOG("ReadColoursSection() sCombo=" << sCombo);
                }

                OSU_WRITESTR("SliderTrackOverride", cov, sSliderTrackOverride, cov.val);
                OSU_WRITESTR("SliderBorder"       , cov, sSliderBorder       , cov.val);
            }
        } else return CFILE_ERR;

        colsec.Combo               = ParseComaSep(sCombo);
        colsec.SliderTrackOverride = ParseComaSep(sSliderTrackOverride);
        colsec.SliderBorder        = ParseComaSep(sSliderBorder);

        return CFILE_OK;
    }

    std::vector<uint8_t> OsuFile::ParseComaSep(std::string in)
    {
        if (in.empty())
            return {};

        std::vector<uint8_t> Out;
        std::string bfr;

        for (int i = 0; i < in.length(); i++)
        {
            if (in[i] == ',')
            {
                Out.push_back(std::stoi(bfr));
                bfr.clear();
            } else bfr += in[i];
        }

        return Out;
    }
}

