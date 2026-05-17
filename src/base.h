/*
 * MIT License
 * Copyright (c) 2026 crelgd
 */

#pragma once

#define IFEL(val, msg) { if (val) throw std::runtime_error(std::string(msg)); }
#define USTRMAX 2048

#ifdef _DEBUG
#   define LOG(val) std::cout << val << std::endl;
#else
#   define LOG(val)
#endif

typedef enum 
{
    CFILE_OK,
    CFILE_ERR,
    CFILE_END,

    CFILE_NEW_SECTION,
    CFILE_SKIP_NOTH
} cFileErr;

namespace base
{
    class File
    {
    public:
        void load(const char* filename);

        // сначало читать возвращаемое значение а потом ошибку
        // прост может вернуть прошлый результат
        //
        // функция для тех функций которые не могут вернуть код ошибки
        cFileErr GetError();

    protected:
        template <typename T>
        T GetVal()
        {
            T val;
            memcpy(&val, fileData.data() + fileOffset, sizeof(T));
            fileOffset += sizeof(T);

            return val;
        }

    protected:
        size_t fileSize;
        uint64_t fileOffset = 0;
        std::vector<uint8_t> fileData;
        cFileErr err = CFILE_OK;         
    };

    std::wstring ConvertWSTR(const char* in, size_t inLength);
}

