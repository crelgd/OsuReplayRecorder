#include "osr.h"

namespace osr
{
    std::vector<Decompile> ReadDecompile(std::vector<uint8_t>& bfr)
    {
        std::vector<Decompile> outData;
        std::vector<char> numBfr;

        std::vector<int> readed;

        for (int i = 0; i < bfr.size(); i++)
        {
            if (bfr[i] == '|' || bfr[i] == ',') {
                readed.push_back(std::stoi(numBfr.data()));
                numBfr.clear();
            }
            else numBfr.push_back(bfr[i]);

            if (readed.size() == 4)
            {
                Decompile dec; 
                dec.w = readed[0];
                dec.x = readed[1];
                dec.y = readed[2];
                dec.z = readed[3];

                outData.push_back(dec);

                readed.clear();
            }
        }

        return outData;
    }
}
