/*
 * MIT License
 * Copyright (c) 2026 crelgd
 */

#include <iostream>

#include "osr.h"
#include "gui/visual.h"

int main(int argc, char* argv[])
{
    try {auto logger = spdlog::basic_logger_mt("basic_logger", "logs/runtime_err.txt"); }
    catch (spdlog::spdlog_ex& err) {
        std::cout << "SPDLOG wasnt load, " << err.what() << std::endl;
    }

    try
    {
        visual::window hwnd;

        hwnd.run();
    }
    catch(const std::runtime_error& err)
    {
        spdlog::error(err.what());
        return 1;
    }
    
    return 0;
}
