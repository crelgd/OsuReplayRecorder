#include <iostream>
#include "osr.h"

int main(int argc, char* argv[])
{
    if (argc < 2) return 0;

    if (strcmp(argv[1], "e") == 0) 
    {
        std::vector<char> arr = uleb128_encode(atoi(argv[2]));
        for (int i = 0; i < arr.size(); i++)
        {
            std::cout << std::hex << int(arr[i]) << "  ";
        }

        std::cout << std::endl;
    }   
    else
    {
        std::cout << "dopustimy tolko e, d" << std::endl;
        return 0;
    }

    return 0;
}

