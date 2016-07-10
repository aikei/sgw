#ifndef SGW_OCTAHEDRON_INCLUDED
#define SGW_OCTAHEDRON_INCLUDED

#include "Triangle.hpp"
#include <sgw/geom/Size3D.hpp>
#include <renderer/shape/BaseShape.hpp>
#include <vector>

namespace sgw
{
    

class Octahedron : public BaseShape
{
public:
    
    Octahedron() {}
    
    Octahedron(float centerX, float centerY, float centerZ, float width, float height, float depth)
    {
        InitOctahedron(Vec3(centerX, centerY, centerZ), Size3D(width, height, depth));
    }
      
    Octahedron(const Vec3& center, float width, float height, float depth)
    {
        InitOctahedron(center, Size3D(width,height,depth));
    }
    
    Octahedron(const Vec3& center, Size3D size)
    {
        InitOctahedron(center, size);
    }    
    /**
     * \brief Set whether this octahedron should be drawn filled or not
     * 
     * @param filled *true* for a filled and *false* (default) for a non-filled octahedron
     */
    void SetFilled(bool filled);
    
protected:
    
    std::vector<Triangle> m_triangles;
    
    void InitOctahedron(const Vec3& center, const Size3D& size);
};


}

#endif


