#ifndef SGW_TRANSFORM_HPP_INCLUDED
#define SGW_TRANSFORM_HPP_INCLUDED

#include <math/math.hpp>
#include <sgw/geom/Vec3.hpp>
#include <vector>
#include <renderer/Vertex.hpp>

namespace sgw
{

struct Vec3;

class Transform
{
public:
    Transform();
    void SetRotation(float rotationX, float rotationY, float rotationZ);
    void SetScale(float scaleX, float scaleY, float scaleZ);
    void SetTranslation(float x, float y, float z);
    void SetTranslation(const Vec3& translation);
    Vec3 Apply(const Vec3& v) const;
    std::vector<Vertex> Apply(const std::vector<Vertex>& vertices) const;
private:
    void MakeFullTransform();
    Mat4 m_fullTransform;

    Mat4 m_rotation;
    Mat4 m_scale;
    Mat4 m_translation;
};


}

#endif
