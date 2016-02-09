#ifndef VEC2_HPP_INCLUDED
#define VEC2_HPP_INCLUDED

#include "Size.hpp"

namespace sgw
{


struct Vec2
{
    float x, y;
    
    Vec2(float xy = 0)
    {
        x = y = xy;
    }
    
    Vec2(float x, float y)
    {
        this->x = x;
        this->y = y;
    }
    
    Vec2 operator-(const Size& size) const
    { 
        Vec2 ret;
        ret.x = x-size.width;
        ret.y = y-size.height;
        return ret;
    }

    Vec2& operator-=(const Size& size)
    {
        x -= size.width;
        y -= size.height;
        return *this;
    }
    
    Vec2 operator+(const Size& size) const
    { 
        Vec2 ret;
        ret.x = x+size.width;
        ret.y = y+size.height;
        return ret;
    }

    Vec2& operator+=(const Size& size)
    {
        x += size.width;
        y += size.height;
        return *this;
    }
    
    Vec2 operator-(const Vec2& other) const
    { 
        Vec2 ret;
        ret.x = x-other.x;
        ret.y = y-other.y;
        return ret;
    }

    Vec2& operator-=(const Vec2& other)
    {
        x -= other.x;
        y -= other.y;
        return *this;
    }
    
    Vec2 operator+(const Vec2& other) const
    { 
        Vec2 ret;
        ret.x = x+other.x;
        ret.y = y+other.y;
        return ret;
    }

    Vec2& operator+=(const Vec2& other)
    {
        x += other.x;
        y += other.y;
        return *this;
    }
    
};


}

#endif
