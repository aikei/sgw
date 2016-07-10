#ifndef VEC3_HPP_INCLUDED
#define VEC3_HPP_INCLUDED

#include <math.h>
#include <cstring>
#include "Vec2.hpp"
#include "Size3D.hpp"

namespace sgw
{


struct Vec3
{
    float x;
    float y;
    float z;

    Vec3(float x=0.0f, float y=0.0f, float z=0.0f)
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }
    
    Vec3(const Vec2& vec2)
    {
        this->x = vec2.x;
        this->y = vec2.y;
        this->z = 0;
    }    
    
    Vec3 Cross(const Vec3& v) const
    {
        const float _x = y * v.z - z * v.y;
        const float _y = z * v.x - x * v.z;
        const float _z = x * v.y - y * v.x;

        return Vec3(_x, _y, _z);
    }
    
    float GetLength() const
    {
        return sqrt(x*x+y*y+z*z);
    }
    
    float GetSquaredLength() const
    {
        return x*x+y*y+z*z;
    }   
    
    bool operator==(const float other) const
    {
        if (x == other && y == other && z == other)
            return true;
        return false;
    }

    bool operator==(const Vec3& other) const
    {
        if (x == other.x && y == other.y && z == other.z)
            return true;
        return false;
    }

    bool operator!=(const float other) const
    {
        if (x != other && y != other && z != other)
            return true;
        return false;
    }

    bool operator!=(const Vec3& other) const
    {
        if (x != other.x || y != other.y || z != other.z)
            return true;
        return false;
    }

    Vec3& operator=(const float other)
    {
        x = other; 
        y = other;
        z = other;
        return *this;
    }
    
    Vec3& operator=(const sgw::Vec2& other)
    {
        x = other.x; 
        y = other.y;
        z = 0;
        return *this;
    }   

    Vec3 operator-(const Vec3& other) const
    { 
        Vec3 ret;
        ret.x = x-other.x;
        ret.y = y-other.y;
        ret.z = z-other.z;
        return ret;
    }

    Vec3& operator-=(const Vec3& other)
    {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        return *this;
    }

    Vec3 operator-(const float other) const
    { 
        Vec3 ret;
        ret.x = x-other;
        ret.y = y-other;
        ret.z = z-other;
        return ret;
    }

    Vec3 operator-() const
    { 
        Vec3 ret;
        ret.x = -x;
        ret.y = -y;
        ret.z = -z;
        return ret;
    }

    Vec3 operator+(const Vec3 &other) const
    {
        Vec3 ret;
        ret.x = x+other.x;
        ret.y = y+other.y;
        ret.z = z+other.z;
        return ret;
    }

    Vec3& operator+=(const Vec3& other)
    {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }

    Vec3 operator+(const float& other) const
    { 
        Vec3 ret;
        ret.x = x+other;
        ret.y = y+other;
        ret.z = z+other;
        return ret;
    }
    
    Vec3& operator+=(const float& other)
    { 
        x = x+other;
        y = y+other;
        z = z+other;
        return *this;
    }    

    Vec3 operator*(const Vec3& other) const
    {
        Vec3 ret;
        ret.x = x*other.x;
        ret.y = y*other.y;
        ret.z = z*other.z;
        return ret;
    }

    Vec3& operator*=(const Vec3 &other)
    {
        x *= other.x;
        y *= other.y;
        z *= other.z;
        return *this;
    }

    Vec3 operator*(const float mult) const
    {
        Vec3 ret;
        ret.x = x*mult;
        ret.y = y*mult;
        ret.z = z*mult;
        return ret;
    }
    
    Vec3 operator/(const Vec3& other) const
    {
        Vec3 ret;
        ret.x = x/other.x;
        ret.y = y/other.y;
        ret.z = z/other.z;
        return ret;
    }
    
    Vec3 operator/(const float div) const
    {
        Vec3 ret;
        ret.x = x/div;
        ret.y = y/div;
        ret.z = z/div;
        return ret;
    }   

    Vec3& operator/=(const Vec3& other)
    {
        x /= other.x;
        y /= other.y;
        z /= other.z;
        return *this;
    }

    Vec3& operator/=(const float scalar)
    {
        x /= scalar;
        y /= scalar;
        z /= scalar;
        return *this;
    }
    
    Vec3 operator-(const Size& size) const
    { 
        Vec3 ret;
        ret.x = x-size.width;
        ret.y = y-size.height;
        return ret;
    }

    Vec3& operator-=(const Size& size)
    {
        x -= size.width;
        y -= size.height;
        return *this;
    }
    
    Vec3 operator+(const Size& size) const
    { 
        Vec3 ret;
        ret.x = x+size.width;
        ret.y = y+size.height;
        return ret;
    }

    Vec3& operator+=(const Size& size)
    {
        x += size.width;
        y += size.height;
        return *this;
    }
    
    Vec3 operator-(const Size3D& size) const
    { 
        Vec3 ret;
        ret.x = x-size.width;
        ret.y = y-size.height;
        ret.z = z-size.depth;
        return ret;
    }

    Vec3& operator-=(const Size3D& size)
    {
        x -= size.width;
        y -= size.height;
        x -= size.depth;
        return *this;
    }
    
    Vec3 operator+(const Size3D& size) const
    { 
        Vec3 ret;
        ret.x = x+size.width;
        ret.y = y+size.height;
        ret.z = z+size.depth;
        return ret;
    }

    Vec3& operator+=(const Size3D& size)
    {
        x += size.width;
        y += size.height;
        z += size.depth;
        return *this;
    }       
    
    Vec3& Normalize()
    {
        float length = GetLength();
        if (length == 0)
            return *this;
        *this = (*this)/length;
        return *this;
    }
    
    void Rotate(float angle, const Vec3& axis);
};


}

#endif
