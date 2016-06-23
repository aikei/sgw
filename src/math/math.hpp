#ifndef SGW_MATH_HPP_INCLUDED
#define SGW_MATH_HPP_INCLUDED

#include <stdio.h>
#include <math.h>
#include <sgw/geom/Vec2.hpp>
#include <sgw/geom/Vec3.hpp>

namespace sgw
{

const float PI = 3.14159265359;

struct Vec4
{
    float x;
    float y;
    float z;
    float w;

    Vec4(float x=0.0f, float y=0.0f, float z=0.0f, float w=0.0f)
    {
        this->x = x;
        this->y = y;
        this->z = z;
        this->w = w;
    }
};

struct Mat4
{
    float m_elements[4][4];
    
    Mat4() { }
    
    Mat4(float e00, float e01, float e02, float e03,
        float e10, float e11, float e12, float e13,
        float e20, float e21, float e22, float e23,
        float e30, float e31, float e32, float e33)
    {
        m_elements[0][0] = e00; m_elements[0][1] = e01; m_elements[0][2] = e02; m_elements[0][3] = e03;
        m_elements[1][0] = e10; m_elements[1][1] = e11; m_elements[1][2] = e12; m_elements[1][3] = e13;
        m_elements[2][0] = e20; m_elements[2][1] = e21; m_elements[2][2] = e22; m_elements[2][3] = e23;
        m_elements[3][0] = e30; m_elements[3][1] = e31; m_elements[3][2] = e32; m_elements[3][3] = e33;
    }
    
    bool operator==(const Mat4& right)
    {
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                if (m_elements[i][j] != right.m_elements[i][j])
                        return false;
        return true;
    }

    bool operator!=(const Mat4& right)
    {
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                if (m_elements[i][j] != right.m_elements[i][j])
                        return true;
        return false;
    }

    Mat4 operator+(const Mat4& right) const
    {
        Mat4 mat;
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
            {
                mat.m_elements[i][j] = m_elements[i][j]+right.m_elements[i][j];
            }
        return mat;
    }

    Mat4 operator+=(const Mat4& right)
    {
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
            {
                m_elements[i][j]+=right.m_elements[i][j];
            }
        return *this;
    }

    Mat4 operator-(const Mat4& right) const
    {
        Mat4 mat;
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
            {
                mat.m_elements[i][j] = m_elements[i][j]-right.m_elements[i][j];
            }
        return mat;
    }

    Mat4 operator-=(const Mat4& right)
    {
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
            {
                m_elements[i][j]-=right.m_elements[i][j];
            }
        return *this;
    }

    Mat4 operator*(const Mat4& right) const
    {
        Mat4 mat;

        for(int i = 0; i < 4; i++)
        {
            for(int j = 0; j < 4; j++)
            {
                mat.m_elements[i][j] = 
                            m_elements[i][0] * right.m_elements[0][j] + 
                            m_elements[i][1] * right.m_elements[1][j] + 
                            m_elements[i][2] * right.m_elements[2][j] + 
                            m_elements[i][3] * right.m_elements[3][j];
            }
        }

        return mat;
    }

    Mat4 operator*=(const Mat4& right)
    {
        *this = (*this) * right;
        return *this;
    }

    Mat4 operator*(const float scalar) const
    {
        Mat4 mat;
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                mat.m_elements[i][j] = m_elements[i][j]*scalar;
            }
        }
        return mat;
    }

    Mat4 operator*=(const float scalar)
    {
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                m_elements[i][j] *= scalar;
            }
        }
        return *this;
    }

    Mat4 operator/(const float scalar) const
    {
        Mat4 mat;
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                mat.m_elements[i][j] = m_elements[i][j]/scalar;
            }
        }
        return mat;
    }

    Mat4 operator/=(const float scalar)
    {
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                m_elements[i][j] /= scalar;
            }
        }
        return *this;
    }

    Vec3 operator*(const Vec3& right) const
    {
        Vec3 point;
        point.x = m_elements[0][0]*right.x+m_elements[0][1]*right.y+m_elements[0][2]*right.z;
        point.y = m_elements[1][0]*right.x+m_elements[1][1]*right.y+m_elements[1][2]*right.z;
        point.z = m_elements[2][0]*right.x+m_elements[2][1]*right.y+m_elements[2][2]*right.z;

        return point;
    }

    Mat4& operator=(const Mat4& other)
    {
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                m_elements[i][j] = other.m_elements[i][j];
        return *this;
    }
        
    void MakeIdentity()
    {
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                if (i == j)
                    m_elements[i][j] = 1.0f;
                else
                    m_elements[i][j] = 0.0f;
            }
        }
    }
    
    //~ void SetRotationZ(float angle);
    //~ void SetRotationX(float angle);
    //~ void SetRotationY(float angle); 
    void SetRotation(float angleX, float angleY, float angleZ);
    void SetScale(float x, float y, float z);
    void SetTranslation(float x, float y, float z);
    void SetPerspectiveProjection(float w, float h, float fov=PI/4,float zNear=1.0f,float zFar=100.0f); 
    void SetOpenGLOrthographicProjection(float w, float h, float zNear, float zFar);
    void SetCameraTransform(const Vec3& target, const Vec3& upAxis);
    void AddRotation(float angleX, float angleY, float angleZ);
    void AddScale(float x, float y, float z);
    void AddTranslation(float x, float y, float z);
    void AddPerspectiveProjection(float w, float h, float fov=PI/4,float zNear=1.0f,float zFar=100.0f); 
};

struct Quaternion
{
    float x, y, z, w;

    Quaternion(float _x, float _y, float _z, float _w);

    void Normalize();

    Quaternion Conjugate();
    
    Quaternion operator*(const Quaternion& r);

    Quaternion operator*(const Vec3& v);
    
};

//~ struct Vertex
//~ {
    //~ Vec3 pos;
    //~ Vec2 texCoords;
    
    //~ Vertex() {}
    
    //~ Vertex(const Vec3& _pos, const Vec2& _texCoords)
    //~ {
        //~ pos = _pos;
        //~ texCoords = _texCoords;
    //~ }
//~ };

}

#endif
