#ifndef SGW_LOGGER_HPP
#define SGW_LOGGER_HPP

#include <cstdarg>
#include <vector>

namespace sgw
{

class Vec3;
class Vertex;
class Mat4;

class Logger
{
public:
    
    enum
    {
        LOG_LEVEL_DEBUG = 0,
        LOG_LEVEL_TRACE = 1,
        LOG_LEVEL_INFO = 2,
        LOG_LEVEL_ERROR = 3,
        LOG_LEVEL_ALWAYS = 99
    };
    
    static int LogLevel;
    
    static void FLog(int level, const char* fmt, ...);
    static void FLog(int level, const Vec3& v);
    static void FLog(int level, const Vertex& vx);
    static void FLog(int level, const std::vector<Vertex>& vertices);
    static void FLog(int level, const std::vector<Vec3>& points);
    static void FLog(int level, const std::vector<unsigned int>& indices);
    static void FLog(int level, const Mat4& mat);
    
    static void FLogTrace(const char* fmt, ...);
    static void FLogTrace(const Vec3& point);
    static void FLogTrace(const std::vector<Vec3>& points);
    static void FLogTrace(const std::vector<Vertex>& vertices);
    
    static void FLogInfo(const char* fmt, ...);
    static void FLogInfo(const Vec3& point);
    static void FLogInfo(const std::vector<Vec3>& points);
    static void FLogInfo(const std::vector<Vertex>& vertices);
          
    static void FLogError(const char* fmt, ...);
    static void FLogAlways(const char* fmt, ...);
    static void FLogDebug(const char* fmt, ...);
    static void FLogNL(int level, const char* fmt, ...);

protected:
    static void FLog(int level, const char* fmt, va_list args);
};
    
}

#endif
