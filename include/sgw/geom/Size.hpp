#ifndef SIZE_HPP_INCLUDED
#define SIZE_HPP_INCLUDED

namespace sgw
{


struct Size
{
    float width, height;
    Size() { width = 0; height = 0; }
    Size(float w, float h) { width = w; height = h; }
    
    Size operator/(float scalar) const
    {
        return Size(width/scalar, height/scalar);
    }
};


}

#endif
