#ifndef SGW_GLFWAPP_HPP_INCLUDED
#define SGW_GLFWAPP_HPP_INCLUDED

#include "BaseApp.hpp"

namespace sgw
{


class GlfwApp: public BaseApp
{
public:
    bool Init(int backend=0);
    void Render();
    void ProcessEvents();
};


}
#endif /* SGW_GLFWAPP_HPP_INCLUDED */ 
