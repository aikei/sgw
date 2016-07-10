#include <sgw/geom/Octahedron.hpp>
#include <sgw/geom/Triangle.hpp>
#include <renderer/FlushList.hpp>

void sgw::Octahedron::InitOctahedron(const Vec3& center, const Size3D& size)
{
    SetCenter(center);
    m_type = SHAPE_OCTAHEDRON;
    m_size = size;
    m_triangles.reserve(8);
    
    //top
    
    m_triangles.emplace_back(
        center.x, center.y-size.height, center.z,
        center.x+size.width/2.0f, center.y, center.z-size.depth/2.0f,
        center.x-size.width/2.0f, center.y, center.z-size.depth/2.0f
    );
    
    m_triangles.emplace_back(
        center.x, center.y-size.height, center.z,
        center.x-size.width/2.0f, center.y, center.z+size.depth/2.0f,
        center.x+size.width/2.0f, center.y, center.z+size.depth/2.0f
    );
    
    m_triangles.emplace_back(
        center.x, center.y-size.height, center.z,
        center.x-size.width/2.0f, center.y, center.z-size.depth/2.0f,
        center.x-size.width/2.0f, center.y, center.z+size.depth/2.0f
    );
        
    m_triangles.emplace_back(
        center.x, center.y-size.height, center.z,
        center.x+size.width/2.0f, center.y, center.z+size.depth/2.0f,
        center.x+size.width/2.0f, center.y, center.z-size.depth/2.0f
    );
        
    //bottom
    
    m_triangles.emplace_back(
        center.x, center.y+size.height, center.z,
        center.x-size.width/2.0f, center.y, center.z-size.depth/2.0f,
        center.x+size.width/2.0f, center.y, center.z-size.depth/2.0f
    );
    
    m_triangles.emplace_back(
        center.x, center.y+size.height, center.z,
        center.x+size.width/2.0f, center.y, center.z+size.depth/2.0f,
        center.x-size.width/2.0f, center.y, center.z+size.depth/2.0f
    );
    
    m_triangles.emplace_back(
        center.x, center.y+size.height, center.z,
        center.x-size.width/2.0f, center.y, center.z+size.depth/2.0f,
        center.x-size.width/2.0f, center.y, center.z-size.depth/2.0f
    );
        
    m_triangles.emplace_back(
        center.x, center.y+size.height, center.z,
        center.x+size.width/2.0f, center.y, center.z-size.depth/2.0f,
        center.x+size.width/2.0f, center.y, center.z+size.depth/2.0f
    );
        
    unsigned int sz = m_triangles.size();
    m_points.reserve(sz*3);
    for (unsigned int i = 0; i < sz; i++)
    {
        std::vector<Vertex> points = m_triangles[i].GetAbsVertices();
        for (unsigned int j = 0; j < 3; j++)
        {
            points[j] = points[j]-m_center;
        }
        m_points.insert(m_points.end(),points.begin(),points.end());
    }
}

void sgw::Octahedron::SetFilled(bool filled)
{
    BaseShape::SetFilled(filled);
    if (m_bFilled)
        m_flushListType = FlushList::TYPE_FILLED_TRIANGLES;
    else
        m_flushListType = FlushList::TYPE_TRIANGLES; 
}
