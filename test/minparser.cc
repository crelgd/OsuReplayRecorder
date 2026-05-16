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

    while (CFILE_END != file.GetError())
    {
        std::string i = file.ParserGetSection();
        if (i.empty())
            std::cout << "section err: " << file.GetError() << std::endl;
        else std::cout << "section: " << i.c_str() << std::endl;

        file.ParserClearSection();

        while (CFILE_NEW_SECTION != file.GetError() )
        {
            if (CFILE_END == file.GetError())
            {
                std::cout << "CFILE_END != file.GetError()\n";
                break;
            }

            ConfOsuValue val = file.ParserGetValue();

            if (val.name.empty())
            {
                std::cout << "val.name.empty() " << file.GetError() << "\n";
                break;
            }

            std::cout << "name:" << val.name.c_str() <<
                " | val:" << val.val.c_str() <<
            std::endl; 
        }
    }

    return 0;
}

