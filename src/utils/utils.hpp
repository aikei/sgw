#ifndef SGW_UTILS_HPP_INCLUDED
#define SGW_UTILS_HPP_INCLUDED

#include <string>
#include <cstdint>

#ifdef PRINT_DEBUG
    #define DEBUG_PRINT(text) printf("%s\n",text);
#else
    #define DEBUG_PRINT(text)
#endif

namespace sgw
{

    class Mat4;
    struct Vec3;
    struct Size;
    
    namespace Utils
    {
        std::string FileToString(const char* fileName);
        float DegreesToRadians(float degrees);
        void DebugPrintMatrix(const Mat4& mat,const std::string& msg="matrix:");
        void DebugPrintVector(const Vec3& v, const std::string& msg);
        float Clamp(float value, float minOriginal, float maxOriginal, float minNew, float maxNew);
        Vec3 ClampVectorFromScreenSize(const Vec3& v, const Size& size, float minNew, float maxNew);
        uint32_t ReverseByteOrder32(uint32_t var);
        uint64_t ReverseByteOrder64(uint64_t var);
        uint32_t ToBigEndian(uint32_t var);
        uint32_t ToHostEndian(uint32_t var);   
        //void DebugPrint(const std::string& text);
    }

}

#endif
