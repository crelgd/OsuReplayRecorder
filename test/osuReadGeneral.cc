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

    file.ParserClearSection();

    OsuGlobalSection ogs;
    OsuMetadataSection oms;

    cFileErr err = file.ReadGeneralSection(ogs);

    if (err != CFILE_OK)
    {
        std::cout << "err code: " << err << std::endl;
        return 1;
    }

    err = file.ReadMetadataSection(oms);

    if (err != CFILE_OK)
    {
        std::cout << "err code meta: " << err << std::endl;
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
        "WidescreenStoryboard: " << ogs.WidescreenStoryboard << "\n" <<
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

    return 0;
}

