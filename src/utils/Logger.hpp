#ifndef SGW_LOGGER_HPP
#define SGW_LOGGER_HPP

#include <cstdarg>

namespace sgw
{
    
class Logger
{
public:
    
    enum
    {
        LOG_LEVEL_TRACE = 0,
        LOG_LEVEL_INFO = 1,
        LOG_LEVEL_DEBUG = 2,
        LOG_LEVEL_ERROR = 3,
        LOG_LEVEL_ALWAYS = 99
    };
    
    static int LogLevel;
    
    static void FLog(int level, const char* fmt, ...);
    static void FLogTrace(const char* fmt, ...);
    static void FLogInfo(const char* fmt, ...);
    static void FLogError(const char* fmt, ...);
    static void FLogAlways(const char* fmt, ...);
    static void FLogDebug(const char* fmt, ...);
    static void FLogNL(int level, const char* fmt, ...);

protected:
    static void FLog(int level, const char* fmt, va_list args);
};
    
}

#endif
