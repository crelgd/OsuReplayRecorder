/*
 * MIT License
 * Copyright (c) 2026 crelgd
 */

#include <fstream>

#include "base.h"

namespace base
{
    void File::load(const char* filename)
    {
        std::ifstream file(filename, std::ios::in | std::ios::binary);

        file.seekg(0, std::ios::end);
        fileSize = file.tellg();
        file.seekg(0, std::ios::beg);

        std::vector<uint8_t> tv(fileSize);
        file.read(reinterpret_cast<char*>(tv.data()), fileSize);

        fileData = tv;
    }

    cFileErr File::GetError()
    {
        return err;
    }

    std::wstring ConvertWSTR(const char* in, size_t inLength)
    {
        size_t i = inLength+1;
        size_t outSize;
        std::wstring Unicode;
        Unicode.resize(i);

    	errno_t err = mbstowcs_s(
            &outSize,
    		&Unicode[0],
    		i,
    		in,
    		i-1);
    	if (err != 0)
    		return {};

    	return Unicode;
    }
}

