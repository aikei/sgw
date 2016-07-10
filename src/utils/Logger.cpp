#include <cstdio>
#include <renderer/Vertex.hpp>
#include <math/math.hpp>
#include "Logger.hpp"
#include <string>

int sgw::Logger::LogLevel = sgw::Logger::LOG_LEVEL_ERROR;
//int sgw::Logger::LogLevel = sgw::Logger::LOG_LEVEL_TRACE;
//int sgw::Logger::LogLevel = sgw::Logger::LOG_LEVEL_DEBUG;

void sgw::Logger::FLog(int level, const char* fmt, va_list args)
{
    if (level < LogLevel)
    {
        va_end(args);
        return;    
    }  
    char buffer[512];
    vsnprintf(buffer, 512, fmt, args);
    printf("%s\n",buffer);
    va_end(args);
}

void sgw::Logger::FLog(int level, const char* fmt, ...)
{
    va_list args;
    va_start (args, fmt);
    FLog(level,fmt,args);
}

void sgw::Logger::FLog(int level, const Vec3& v)
{
    FLog(level,"{ %f, %f, %f }",v.x,v.y,v.z);
}

void sgw::Logger::FLog(int level, const Vertex& vx)
{
    FLog(level,"Vx, pos: { %f, %f, %f }, uv: { %f, %f }, clr: %d:%d:%d:%d",vx.pos.x,vx.pos.y,vx.pos.z,vx.texCoord.x,vx.texCoord.y,vx.clr.r,vx.clr.g,vx.clr.b,vx.clr.a);
}

void sgw::Logger::FLog(int level, const std::vector<Vec3>& points)
{
    FLog(level,"Points: ");
    unsigned int sz = points.size();
    for (unsigned int i = 0; i < sz; i++)
    {
        FLog(level,points[i]);
    }
}

void sgw::Logger::FLog(int level, const std::vector<unsigned int>& indices)
{
    unsigned int sz = indices.size();
    FLog(level,"Indices, size: %d",sz);
    for (unsigned int i = 0; i < sz; i++)
    {
        FLog(level,"%d",indices[i]);
    }    
}

void sgw::Logger::FLog(int level, const std::vector<Vertex>& vertices)
{
    unsigned int sz = vertices.size();
    FLog(level,"Vertices, size: %d",sz);
    for (unsigned int i = 0; i < sz; i++)
    {
        FLog(level,vertices[i]);
    }
}

void sgw::Logger::FLog(int level, const Mat4& mat)
{
    FLog(level,"Matrix: ");
    for (unsigned int i = 0; i < 4; i++)
    {
        FLog(level,"%f %f %f %f",mat.m_elements[i][0],mat.m_elements[i][1],mat.m_elements[i][2],mat.m_elements[i][3]);
    }
}

void sgw::Logger::FLogNL(int level, const char* fmt, ...)
{  
    char buffer[512];
    va_list args;
    va_start (args, fmt);
    FLog(level,fmt,args);
    printf("%s","\n");
}

void sgw::Logger::FLogTrace(const char* fmt, ...)
{
    va_list args;
    va_start (args, fmt);    
    FLog(LOG_LEVEL_TRACE,fmt,args);
}

void sgw::Logger::FLogInfo(const char* fmt, ...)
{
    va_list args;
    va_start (args, fmt);    
    FLog(LOG_LEVEL_INFO,fmt,args);    
}

void sgw::Logger::FLogInfo(const Vec3& point)
{
    FLog(LOG_LEVEL_INFO,point);    
}

void sgw::Logger::FLogInfo(const std::vector<Vec3>& points)
{
    FLog(LOG_LEVEL_INFO,points);    
}

void sgw::Logger::FLogInfo(const std::vector<Vertex>& vertices)
{
    FLog(LOG_LEVEL_INFO,vertices);    
}

void sgw::Logger::FLogTrace(const Vec3& point)
{
    FLog(LOG_LEVEL_TRACE,point);    
}

void sgw::Logger::FLogTrace(const std::vector<Vec3>& points)
{
    FLog(LOG_LEVEL_TRACE,points);    
}

void sgw::Logger::FLogTrace(const std::vector<Vertex>& vertices)
{
    FLog(LOG_LEVEL_TRACE,vertices);    
}

void sgw::Logger::FLogError(const char* fmt, ...)
{
    va_list args;
    va_start (args, fmt);    
    FLog(LOG_LEVEL_ERROR,fmt,args);    
}

void sgw::Logger::FLogAlways(const char* fmt, ...)
{
    va_list args;
    va_start (args, fmt);    
    FLog(LOG_LEVEL_ALWAYS,fmt,args);    
}

void sgw::Logger::FLogDebug(const char* fmt, ...)
{
    va_list args;
    va_start (args, fmt);    
    FLog(LOG_LEVEL_DEBUG,fmt,args);      
}
