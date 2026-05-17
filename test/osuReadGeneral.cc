#include <iostream>
#include <vector>
#include <locale>   
#include <windows.h>
#include "file/osu.h"

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cout <<
            "Usage: orr <replay_name>" <<
        std::endl;

        return 0;
    }


    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    std::locale::global(std::locale(".UTF-8"));
    std::wcout.imbue(std::locale());

    osu::OsuFile file;
    file.load(argv[1]);

    file.GetAllSections();

    file.ParserClearSection();

    OsuGlobalSection ogs;
    OsuMetadataSection oms;
    OsuDifficultSection ods;
    OsuColoursSection ocs;

    cFileErr err1 = file.ReadGeneralSection(ogs);
    cFileErr err2 = file.ReadMetadataSection(oms);
    cFileErr err3 = file.ReadDifficultSection(ods);
    cFileErr err4 = file.ReadColoursSection(ocs);

    if (err1 != CFILE_OK || err2 != CFILE_OK || err3 != CFILE_OK || err4 != CFILE_OK)
    {
        std::cout << err1 << " " << err2 << " " << err3 << " " << err4 << std::endl;
        return 1;
    }

    std::cout << "\tSection General\n" << 
        "AudioFileName: " << ogs.AudioFileName << "\n" <<
        "AudioLeadIn: " << ogs.AudioLeadIn << "\n" <<
        "AudioHash: " << ogs.AudioHash << "\n" <<
        "PreviewTime: " << ogs.PreviewTime << "\n" <<
        "Countdown: " << ogs.Countdown << "\n" <<
        "SampleSet: " << ogs.SampleSet << "\n" <<
        "StackLeniency: " << ogs.StackLeniency << "\n" <<
        "Mode: " << ogs.Mode << "\n" <<
        "LetterboxInBreaks: " << ogs.LetterboxInBreaks << "\n" <<
        "StoryFileInFront: " << ogs.StoryFileInFront << "\n" << 
        "UserSkinSprites: " <<  ogs.UseSkinSprites << "\n" <<
        "AlwaysShowPlayfield: " << ogs.AlwaysShowPlayfield << "\n" <<
        "OverlayPosition: " << ogs.OverlayPosition << "\n" <<
        "SkinPreference: " << ogs.SkinPreference << "\n" << 
        "EpilepcyWarning: " << ogs.EpilepcyWarning << "\n" <<
        "Countdownoffset: " << ogs.CountdownOffset << "\n" << 
        "SpecialStyle: " << ogs.SpecialStyle << "\n" <<
        "WidescreenStoryboard: " << ogs.WidescreenStoryboard <<
    std::endl;

    std::cout << "\tSection Metadata\n" << 
        "Title: " << oms.Title << "\n";
    std::wcout << "TitleUnicode: " << oms.TitleUnicode << "\n";
    std::cout << "Artist: " << oms.Artist << "\n";
    std::wcout << "ArtistUnicode: " << oms.ArtistUnicode << "\n";
    std::cout << "Creator: " << oms.Creator << "\n" <<
        "Version: " << oms.Version << "\n" <<
        "Source: " << oms.Source <<
    std::endl;

    for (int i = 0; i < oms.Tags.size(); i++)
    {
        std::cout << "Tag: " << oms.Tags[i] << "\n";
    }

    std::cout <<
        "BeatmapID: " << oms.BeatmapID << "\n" <<
        "BeatmapSetID: " << oms.BeatmapSetID << "\n" <<
    std::endl;

    std::cout << "\tSection Difficult\n" <<
        "HPDrainRate: " << float(ods.HPDrainRate) << "\n" <<
        "CircleSize: " << float(ods.CircleSize) << "\n" <<
        "OverallDiffculty: " << float(ods.OverallDifficulty) << "\n" <<
        "ApproachRate: " << float(ods.ApproachRate) << "\n" <<
        "SliderMultiplier: " << float(ods.SliderMultiplier) << "\n" <<
        "SliderTickRate: " << float(ods.SliderTickRate) <<
    std::endl;

    std::cout << "\tSection Colour\nCombo: " << std::endl;
    for (int i = 0; i < ocs.Combo.size(); i++)
    {
        std::cout << int(ocs.Combo[i]) << ", ";
    }
    std::cout << "\nSliderTrackOverride: ";
    for (int i = 0; i < ocs.SliderTrackOverride.size(); i++)
    {
        std::cout << int(ocs.SliderTrackOverride[i]) << ", ";
    }
    std::cout << "\nSliderBorder: ";
    for (int i = 0; i < ocs.SliderBorder.size(); i++)
    {
        std::cout << int(ocs.SliderBorder[i]) << ", ";
    }
    
    if (1 > ods.SliderMultiplier) std::cout << "3214214fw\n";

    return 0;
}

// сломаны float