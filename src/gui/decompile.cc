#include "osr.h"

namespace osr
{
    std::vector<Decompile> ReadDecompile(std::vector<uint8_t>& bfr)
    {
        std::vector<Decompile> outData;
        std::vector<char> numBfr;

        std::vector<float> readed;

        for (int i = 0; i < bfr.size(); i++)
        {
            if (bfr[i] == '|' || bfr[i] == ',') {
                std::string tmp(numBfr.begin(), numBfr.end());
                readed.push_back(std::stof(tmp));
                numBfr.clear();
            }
            else numBfr.push_back(bfr[i]);

            if (readed.size() == 4)
            {
                Decompile dec; 
                dec.w = int(readed[0]);
                dec.x = readed[1];
                dec.y = readed[2];
                dec.z = int(readed[3]);
                dec.readed = false;

                outData.push_back(dec);

                readed.clear();
            }
        }

        return outData;
    }
}
