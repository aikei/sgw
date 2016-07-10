#include "Octahedron.hpp"
#include <memory>
#include <sgw/Texture.hpp>

namespace sgw
{
    

class Sphere : public Octahedron
{
public:

    Sphere(float x, float y, float z, float radius, unsigned int splits=4) 
    {
        InitOctahedron(Vec3(x,y,z), Size3D(radius, radius, radius));
        InitSphere(Vec3(x,y,z), radius, splits);
        m_type = BaseShape::SHAPE_SPHERE;
    }
    
    Sphere(const Vec3& center, float radius, unsigned int splits=4) 
    {
        InitOctahedron(center, Size3D(radius, radius, radius));
        InitSphere(center, radius, splits);
        m_type = BaseShape::SHAPE_SPHERE;
    }
    
protected:
    float m_radius;
    void InitSphere(const Vec3& center, float radius, unsigned int splits=4);
};   
    
    
}
