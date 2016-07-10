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
    
    Vertex(const Vec3& pos,const Vec2& texCoord=Vec2(0,0)) : clr(0xFFFFFFFF)
    {
        this->pos = pos;
        this->texCoord = texCoord;
    }
    
    Vertex operator-(const Vec3& other) const
    { 
        
        return Vertex(pos-other, texCoord);
    }

    Vertex& operator-=(const Vec3& other)
    {
        pos -= other;
        return *this;
    }

    Vertex operator-(const float other) const
    { 
        return Vertex(pos-other, texCoord);
    }

    Vertex operator-() const
    { 
        return Vertex(-pos,texCoord);
    }

    Vertex operator+(const Vec3 &other) const
    {
        return Vertex(pos+other,texCoord);
    }

    Vertex& operator+=(const Vec3& other)
    {
        pos += other;
        return *this;
    }

    Vertex operator+(const float& other) const
    { 
        return Vertex(pos+other,texCoord);
    }
    
    Vertex& operator+=(const float& other)
    { 
        pos += other;
        return *this;
    }    

    Vertex operator*(const Vec3& other) const
    {
        return Vertex(pos*other,texCoord);
    }

    Vertex& operator*=(const Vec3 &other)
    {
        pos *= other;
        return *this;
    }

    Vertex operator*(const float mult) const
    {
        return Vertex(pos*mult,texCoord);
    }
    
    Vertex operator/(const Vec3& other) const
    {
        return Vertex(pos/other,texCoord);
    }
    
    Vertex operator/(const float div) const
    {
        return Vertex(pos/div,texCoord);
    }   

    Vertex& operator/=(const Vec3& other)
    {
        pos /= other;
        return *this;
    }

    Vertex& operator/=(const float scalar)
    {
        pos /= scalar;
        return *this;
    }
    
    Vertex operator-(const Size& size) const
    { 
        return Vertex(pos-size,texCoord);
    }

    Vertex& operator-=(const Size& size)
    {
        pos -= size;
        return *this;
    }
    
    Vertex operator+(const Size& size) const
    { 
        return Vertex(pos+size,texCoord);
    }

    Vertex& operator+=(const Size& size)
    {
        pos -= size;
        return *this;
    }
    
    Vertex operator-(const Size3D& size) const
    { 
        return Vertex(pos-size,texCoord);
    }

    Vertex& operator-=(const Size3D& size)
    {
        pos -= size;
        return *this;
    }
    
    Vertex operator+(const Size3D& size) const
    { 
        return Vertex(pos+size,texCoord);
    }

    Vertex& operator+=(const Size3D& size)
    {
        pos += size;
        return *this;
    }       
    
    Vertex& Normalize()
    {
        float length = pos.GetLength();
        if (length == 0)
            return *this;
        pos = pos/length;
        return *this;
    }    
};
    
    
}

#endif
