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

    std::vector<int64_t> d = file.ParserGetComaSeparatedValue();
    for (int i = 0; i < d.size(); i++)
    {
        if (OSU_CODE_CHECK(d[i]))
            std::cout << d[i] << " <-end ! ";
        else std::cout << d[i] << " ! ";

        for (int j = 0; j < 4; j++)
        {
            if (OsuSliderCode[j].code == d[i])
            {
                std::cout << OsuSliderCode[j].type << " ! ";
            }
        }
    }

    std::cout << std::endl;

    return 0;
}

