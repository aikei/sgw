#ifndef SGW_VERTEX_INCLUDED
#define SGW_VERTEX_INCLUDED

#include <sgw/geom/Vec3.hpp>
#include <sgw/geom/Vec2.hpp>
#include <sgw/Color.hpp>
#include <cstdio>

namespace sgw
{
    

struct Vertex
{
    Vec3 pos;
    Color clr;
    Vec2 texCoord;
    
    Vertex() : pos(0,0,0), clr(0xFFFFFFFF)
    {

    }
};
    
    
}

#endif
