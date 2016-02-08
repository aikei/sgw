#include "utils.hpp"

#include <cmath>

#include <math/math.hpp>
#include <app/gApp.hpp>
#include <app/BaseApp.hpp>
#include <sgw/AppData.hpp>

#include <cstdio>
#include <cerrno>
#include <stdio.h>
#include <cstring>
#include <stdexcept>

#ifdef SGW_PLATFORM_LINUX
#include <arpa/inet.h>
#endif

static bool debug = true;

std::string sgw::Utils::FileToString(const char* fileName)
{
    std::string str;
    std::FILE* f = std::fopen(fileName,"r");
    if(f)
    {
        std::fseek(f,0,SEEK_END);
        int size = std::ftell(f);
        std::fseek(f,0,SEEK_SET);
        str.resize(size);
        size_t n = std::fread(&str[0],1,size,f);
        if (n == 0)
            throw std::runtime_error("Read nothing from file!");
        std::fclose(f);
        return str;
    }
    printf("Error reading file to string %d\n",errno);
    throw errno;
}


float sgw::Utils::DegreesToRadians(float degrees)
{
    return degrees*(PI/180.0);
}

void sgw::Utils::DebugPrintVector(const sgw::Vec3& v, const std::string& msg)
{
    std::string beginning = msg+": (%f, %f, %f)\n";
    printf(beginning.c_str(),v.x,v.y,v.z);
}

void sgw::Utils::DebugPrintMatrix(const Mat4& mat,const std::string& msg)
{
    std::string beginning = msg+"\n %1.2f %1.2f %1.2f %1.2f\n %1.2f %1.2f %1.2f %1.2f\n %1.2f %1.2f %1.2f %1.2f\n %1.2f %1.2f %1.2f %1.2f\n";
    printf(beginning.c_str(),
    mat.m_elements[0][0], mat.m_elements[0][1], mat.m_elements[0][2], mat.m_elements[0][3],
    mat.m_elements[1][0], mat.m_elements[1][1], mat.m_elements[1][2], mat.m_elements[1][3],
    mat.m_elements[2][0], mat.m_elements[2][1], mat.m_elements[2][2], mat.m_elements[2][3],
    mat.m_elements[3][0], mat.m_elements[3][1], mat.m_elements[3][2], mat.m_elements[3][3]);
}

float sgw::Utils::Clamp(float value, float minOriginal, float maxOriginal, float minNew, float maxNew)
{
    float range = maxOriginal-minOriginal;
    float share = value/range;
    range = maxNew - minNew;
    return (share*range)+minNew;
}

sgw::Vec3 sgw::Utils::ClampVectorFromScreenSize(const Vec3& v, const Size& size, float minNew, float maxNew)
{
    //const Size& size = gApp->GetAppData().windowSize;
    Vec3 result(0,0,0);
    result.x = Utils::Clamp(v.x, 0, size.width, minNew, maxNew);
    result.y = Utils::Clamp(v.y, 0, size.height, minNew, maxNew);
    return result;
}

uint32_t sgw::Utils::ReverseByteOrder32(uint32_t var)
{
#ifdef SGW_COMPILER_GCC
    return __builtin_bswap32(var);
#endif    
}

uint64_t sgw::Utils::ReverseByteOrder64(uint64_t var)
{
#ifdef SGW_COMPILER_GCC
    return __builtin_bswap64(var);
#endif    
}

uint32_t sgw::Utils::ToBigEndian(uint32_t var)
{
    return htonl(var);
}

uint32_t sgw::Utils::ToHostEndian(uint32_t var)
{
    return ntohl(var);
}

/*
void sgw::Utils::DebugPrint(const std::string& text)
{
    if (debug)
        printf("%s\n",text.c_str());
}
*/
