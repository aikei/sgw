#ifndef SGW_APP_DATA_HPP_INCLUDED
#define SGW_APP_DATA_HPP_INCLUDED

#include <math/math.hpp>
#include <string>
#include "Prefs.hpp"
#include "geom/Size.hpp"

namespace sgw
{

struct AppData
{
    int argc;
    char** argv;
    Vec2 windowPosition;
    Vec2 screenCenter;
    Size windowSize;
    std::string title;
    int videoBackend;
    
    AppData()
    {
        windowPosition = Vec2(0,0);
        windowSize = Size(1024, 768);
        screenCenter.x = windowSize.width/2.0f;
        screenCenter.y = windowSize.height/2.0f;
        title = "Window";
        videoBackend = SGW_CONST_RENDERER_BACKEND_OPEN_GL;
        argc = 0;
        argv = NULL;
    }
};

}

#endif
