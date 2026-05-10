/*
 * MIT License
 * Copyright (c) 2026 crelgd
 */

#include <iostream>

#include "visual/visual.h"
#include "osr.h"

#include "osu.h"

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cout <<
            "Usage: orr <replay_name>" <<
        std::endl;

        return 0;
    }

    // auto logger = spdlog::basic_logger_mt("basic_logger", "logs/runtime_err.txt");
    // spdlog::set_default_logger(logger);

    // try
    // {
    //     visual::window wnd;
    //     wnd.osrSet(argv[1]);
    //     wnd.run();
    // }
    // catch(const std::runtime_error& err)
    // {
    //     spdlog::error(err.what());
    //     return 1;
    // }


    // пример чтения .osu
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
                break;

            ConfOsuValue val = file.ParserGetValue();

            if (val.name.empty())
                break;

            std::cout << "name:" << val.name.c_str() <<
                " | val:" << val.val.c_str() <<
            std::endl; 
        }
    }
    
    return 0;
}
