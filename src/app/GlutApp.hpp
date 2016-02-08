#ifndef SGW_GLUT_APP_HPP_INCLUDED
#define SGW_GLUT_APP_HPP_INCLUDED

#include "BaseApp.hpp"

namespace sgw
{

class GlutApp
    : public BaseApp
{
public:
    bool Init(int backend=0);
    void Render();
    void ProcessEvents();
    void Exit();
};
    
}

#endif
