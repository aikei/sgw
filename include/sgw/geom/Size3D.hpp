#ifndef SIZE3D_HPP_INCLUDED
#define SIZE3D_HPP_INCLUDED

#include "Size.hpp"

namespace sgw
{


struct Size3D
{
    float width, height, depth;
    
    Size3D() { width = 0; height = 0; depth = 0; }
    
    Size3D(float w, float h, float d = 0) { width = w; height = h; depth = d; }
    
    Size3D(const Size& size) { width = size.width; height = size.height; depth = 0; }
    
    Size3D operator=(const Size& size)
    {
        width = size.width;
        height = size.height;
        depth = 0;
    }
    
    Size3D operator/(float scalar) const
    {
        return Size3D(width/scalar, height/scalar, depth/scalar);
    }
};


}

#endif
