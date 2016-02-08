#include <cstdio>
#include "Logger.hpp"

int sgw::Logger::LogLevel = sgw::Logger::LOG_LEVEL_ERROR;

void sgw::Logger::FLog(int level, const char* fmt, va_list args)
{
    if (level < LogLevel)
    {
        va_end(args);
        return;    
    }  
    char buffer[512];
    vsnprintf(buffer, 512, fmt, args);
    printf("%s",buffer);
    va_end(args);
}

void sgw::Logger::FLog(int level, const char* fmt, ...)
{
    va_list args;
    va_start (args, fmt);
    FLog(level,fmt,args);
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
