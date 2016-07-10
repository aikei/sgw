#include <math.h>
#include "math.hpp"
#include <utils/utils.hpp>
#include <utils/Logger.hpp>

void sgw::Vec3::Rotate(float angle, const Vec3& axis)
{
    const float sinHalfAngle = sinf(angle/2);
    const float cosHalfAngle = cosf(angle/2);

    const float Rx = axis.x * sinHalfAngle;
    const float Ry = axis.y * sinHalfAngle;
    const float Rz = axis.z * sinHalfAngle;
    const float Rw = cosHalfAngle;
    Quaternion rotationQ(Rx, Ry, Rz, Rw);

    Quaternion conjugateQ = rotationQ.Conjugate();
    //  ConjugateQ.Normalize();
    Quaternion w = rotationQ * (*this) * conjugateQ;

    x = w.x;
    y = w.y;
    z = w.z;
}

void sgw::Mat4::SetRotation(float angleX, float angleY, float angleZ)
{
    float cosine = cosf(angleX);
    float sine = sinf(angleX);
    Mat4 matx(  1.0f,           0.0f,               0.0f,           0.0f,
                0.0f,           cosine,             sine,           0.0f,
                0.0f,           sine,               cosine,         0.0f,
                0.0f,           0.0f,               0.0f,           1.0f);
    //DebugPrintMatrix(matx,"matx:");
        
    cosine = cosf(angleY);
    sine = sinf(angleY);                
    Mat4 maty(  cosine,         0.0f,               -sine,          0.0f,
                0.0f,           1.0f,               0.0f,           0.0f,
                sine,           0.0f,               cosine,         0.0f,
                0.0f,           0.0f,               0.0f,           1.0f);
    //DebugPrintMatrix(maty,"maty:");   
            
    cosine = cosf(angleZ);
    sine = sinf(angleZ);            
    Mat4 matz(  cosine,         -sine,              0.0f,           0.0f,
                sine,           cosine,             0.0f,           0.0f,
                0.0f,           0.0f,               1.0f,           0.0f,
                0.0f,           0.0f,               0.0f,           1.0f);
    //DebugPrintMatrix(matz,"matz:");
    
    *this = matz*maty*matx;
}

void sgw::Mat4::AddRotation(float angleX, float angleY, float angleZ)
{
    Mat4 mat;
    mat.MakeIdentity();
    mat.SetRotation(angleX, angleY, angleZ);
    *this = mat*(*this);
}

void sgw::Mat4::SetScale(float x, float y, float z)
{
    m_elements[0][0] = x;
    m_elements[1][1] = y;
    m_elements[2][2] = z;           
}

void sgw::Mat4::AddScale(float x, float y, float z)
{
    sgw::Mat4 mat;
    mat.MakeIdentity();
    mat.SetScale(x, y, z);
    *this = mat*(*this);    
}

void sgw::Mat4::SetTranslation(float x, float y, float z)
{
    m_elements[0][3] = x;
    m_elements[1][3] = y;
    m_elements[2][3] = z;
}

void sgw::Mat4::AddTranslation(float x, float y, float z)
{
    m_elements[0][3] += x;
    m_elements[1][3] += y;
    m_elements[2][3] += z;
}

//~ void Mat4::AddTranslation(float x, float y, float z)
//~ {
    //~ Mat4 mat;
    //~ mat.MakeIdentity();
    //~ mat.SetTranslation(x, y, z);
    //~ *this = mat*(*this);    
//~ }

void sgw::Mat4::SetPerspectiveProjection(float w, float h, float fov, float zNear, float zFar)
{
    const float ar = w / h;
    const float zRange = zNear - zFar;
    const float tanHalfFOV = tanf(fov / 2.0f);

    m_elements[0][0] = 1.0f/(tanHalfFOV * ar); m_elements[0][1] = 0.0f;             m_elements[0][2] = 0.0f;                            m_elements[0][3] = 0.0;
    m_elements[1][0] = 0.0f;                   m_elements[1][1] = 1.0f/tanHalfFOV;  m_elements[1][2] = 0.0f;                            m_elements[1][3] = 0.0;
    m_elements[2][0] = 0.0f;                   m_elements[2][1] = 0.0f;             m_elements[2][2] = (-zNear - zFar)/zRange;          m_elements[2][3] = 2.0f*zFar*zNear/zRange;
    m_elements[3][0] = 0.0f;                   m_elements[3][1] = 0.0f;             m_elements[3][2] = 1.0f;                            m_elements[3][3] = 0.0;     
}

void sgw::Mat4::SetOpenGLOrthographicProjection(float w, float h, float zNear, float zFar)
{
    const float zRange = zFar - zNear;

    m_elements[0][0] = 2.0f/w;                  m_elements[0][1] = 0.0f;                m_elements[0][2] = 0.0f;                            m_elements[0][3] = -1.0;
    m_elements[1][0] = 0.0f;                    m_elements[1][1] = -2.0f/h;             m_elements[1][2] = 0.0f;                            m_elements[1][3] = 1.0;
    m_elements[2][0] = 0.0f;                    m_elements[2][1] = 0.0f;                m_elements[2][2] = -2/zRange;                       m_elements[2][3] = (zFar+zNear)/zRange;
    m_elements[3][0] = 0.0f;                    m_elements[3][1] = 0.0f;                m_elements[3][2] = 0.0f;                            m_elements[3][3] = 1.0;     
}

void sgw::Mat4::AddPerspectiveProjection(float w, float h, float fov, float zNear, float zFar)
{
    Mat4 mat;
    mat.SetPerspectiveProjection(w, h, fov, zNear, zFar);
    *this = mat*(*this);    
}

std::vector<sgw::Vec3>& sgw::Mat4::TransformPoints(std::vector<Vec3>& points) const
{
    unsigned int sz = points.size();
    for (int i = 0; i < sz; i++)
    {
        points[i] = (*this)*points[i];
    }
    return points;    
}

void sgw::Mat4::SetCameraTransform(const Vec3& target, const Vec3& upAxis)
{
    Vec3 n = target;
    n.Normalize();
    Vec3 u = upAxis;
    u.Normalize();
    u = u.Cross(target);
    Vec3 v = n.Cross(u);

    m_elements[0][0] = u.x;     m_elements[0][1] = u.y;     m_elements[0][2] = u.z;     m_elements[0][3] = 0.0f;
    m_elements[1][0] = v.x;     m_elements[1][1] = v.y;     m_elements[1][2] = v.z;     m_elements[1][3] = 0.0f;
    m_elements[2][0] = n.x;     m_elements[2][1] = n.y;     m_elements[2][2] = n.z;     m_elements[2][3] = 0.0f;
    m_elements[3][0] = 0.0f;    m_elements[3][1] = 0.0f;    m_elements[3][2] = 0.0f;    m_elements[3][3] = 1.0f;    
}

sgw::Quaternion::Quaternion(float _x, float _y, float _z, float _w)
{
    x = _x;
    y = _y;
    z = _z;
    w = _w;
}

void sgw::Quaternion::Normalize()
{
    float length = sqrtf(x * x + y * y + z * z + w * w);

    x /= length;
    y /= length;
    z /= length;
    w /= length;
}


sgw::Quaternion sgw::Quaternion::Conjugate()
{
    sgw::Quaternion ret(-x, -y, -z, w);
    return ret;
}

sgw::Quaternion sgw::Quaternion::operator*(const sgw::Quaternion& r)
{
    const float ww = (w * r.w) - (x * r.x) - (y * r.y) - (z * r.z);
    const float xx = (x * r.w) + (w * r.x) + (y * r.z) - (z * r.y);
    const float yy = (y * r.w) + (w * r.y) + (z * r.x) - (x * r.z);
    const float zz = (z * r.w) + (w * r.z) + (x * r.y) - (y * r.x);

    sgw::Quaternion ret(xx, yy, zz, ww);
    return ret;
}

sgw::Quaternion sgw::Quaternion::operator*(const sgw::Vec3& v)
{
    const float ww = - (x * v.x) - (y * v.y) - (z * v.z);
    const float xx =   (w * v.x) + (y * v.z) - (z * v.y);
    const float yy =   (w * v.y) + (z * v.x) - (x * v.z);
    const float zz =   (w * v.z) + (x * v.y) - (y * v.x);

    sgw::Quaternion ret(xx, yy, zz, ww);
    return ret;
}
