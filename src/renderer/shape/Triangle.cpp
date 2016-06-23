#include <sgw/geom/Triangle.hpp>
#include <utils/Logger.hpp>

std::vector<sgw::Triangle> sgw::Triangle::Split() const
{
    std::vector<sgw::Triangle> triangles;
    
    Vec3 center01 = (m_points[0]+m_points[1])/2;
    Vec3 center12 = (m_points[1]+m_points[2])/2;
    Vec3 center20 = (m_points[2]+m_points[0])/2;
    sgw::Logger::FLogTrace("center01: { %f, %f, %f }", center01.x, center01.y, center01.z);
    sgw::Logger::FLogTrace("center12: { %f, %f, %f }", center12.x, center12.y, center12.z);
    sgw::Logger::FLogTrace("center20: { %f, %f, %f }", center20.x, center20.y, center20.z);
    triangles.emplace_back(m_points[0],center01,center20);
    triangles.back().SetFilled(this->m_bFilled);
    triangles.emplace_back(m_points[1],center01,center12);
    triangles.back().SetFilled(this->m_bFilled);
    triangles.emplace_back(m_points[2],center12,center20);
    triangles.back().SetFilled(this->m_bFilled);
    triangles.emplace_back(center01,center12,center20);
    triangles.back().SetFilled(this->m_bFilled);
    
    return triangles;
}
