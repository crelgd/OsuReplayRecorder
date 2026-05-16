#include <iostream>
#include <vector>
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

    osu::OsuFile file;
    file.load(argv[1]);

    file.ParserClearSection();

    OsuGlobalSection ogs;

    cFileErr err = file.ReadGeneralSection(ogs);

    if (err != CFILE_OK)
    {
        std::cout << "err code: " << err << std::endl;
        return 1;
    }

    std::cout <<
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

    return 0;
}

