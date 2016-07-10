#include <sgw/geom/Triangle.hpp>
#include <utils/Logger.hpp>

std::vector<sgw::Triangle> sgw::Triangle::Split() const
{
    sgw::Logger::FLogInfo("------Inside Triangle::Split(), m_center: ");
    sgw::Logger::FLog(Logger::LOG_LEVEL_INFO,m_center);
    sgw::Logger::FLogInfo("m_points: ");
    sgw::Logger::FLog(Logger::LOG_LEVEL_INFO,m_points);
    std::vector<sgw::Triangle> triangles;
    
    Vec3 center01 = (m_points[0].pos+m_points[1].pos)/2+m_center;
    Vec3 center12 = (m_points[1].pos+m_points[2].pos)/2+m_center;
    Vec3 center20 = (m_points[2].pos+m_points[0].pos)/2+m_center;
    sgw::Logger::FLogInfo("center01: { %f, %f, %f }", center01.x, center01.y, center01.z);
    sgw::Logger::FLogInfo("center12: { %f, %f, %f }", center12.x, center12.y, center12.z);
    sgw::Logger::FLogInfo("center20: { %f, %f, %f }", center20.x, center20.y, center20.z);
    sgw::Logger::FLogInfo("--Split triangle 1:");
    triangles.emplace_back(m_points[0].pos+m_center,center01,center20);
    triangles.back().SetFilled(this->m_bFilled);
    sgw::Logger::FLogInfo("--Split triangle 2:");
    triangles.emplace_back(m_points[1].pos+m_center,center12,center01);
    triangles.back().SetFilled(this->m_bFilled);
    sgw::Logger::FLogInfo("--Split triangle 3:");
    triangles.emplace_back(m_points[2].pos+m_center,center20,center12);
    triangles.back().SetFilled(this->m_bFilled);
    sgw::Logger::FLogInfo("--Split triangle 4:");
    triangles.emplace_back(center01,center12,center20);
    triangles.back().SetFilled(this->m_bFilled);
    
    return triangles;
}

void sgw::Triangle::InitTriangle(const Vec3& vertex1, const Vec3& vertex2, const Vec3& vertex3)
{
    sgw::Logger::FLogInfo("------Inside Triangle::InitTriangle(), vertices: ");
    sgw::Logger::FLog(Logger::LOG_LEVEL_INFO,vertex1);
    sgw::Logger::FLog(Logger::LOG_LEVEL_INFO,vertex2);
    sgw::Logger::FLog(Logger::LOG_LEVEL_INFO,vertex3);
    SetCenter((vertex1+vertex2+vertex3)/3);
    m_pos = m_center;
    sgw::Logger::FLogInfo("center: ");
    sgw::Logger::FLog(Logger::LOG_LEVEL_INFO,m_center);
    m_points.reserve(3);
    m_points.emplace_back(vertex1-m_center);
    m_points.emplace_back(vertex2-m_center);
    m_points.emplace_back(vertex3-m_center);
    
    sgw::Logger::FLogInfo("points: ");
    sgw::Logger::FLog(Logger::LOG_LEVEL_INFO,m_points); 
}
