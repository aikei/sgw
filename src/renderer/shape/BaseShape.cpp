#include "BaseShape.hpp"
#include <utils/Logger.hpp>

std::vector<sgw::Vertex> sgw::BaseShape::GetAbsVertices() const
{
    std::vector<Vertex> absPoints;
    unsigned int sz = m_points.size();
    absPoints.reserve(sz);
    for (unsigned int i = 0; i < sz; i++)
    {
        absPoints.push_back(m_points[i]+m_center);
    }
    return absPoints;
}

std::vector<sgw::Vertex> sgw::BaseShape::GetConvertedVertices(const Vec3& newCenter) const
{
    std::vector<Vertex> convertedPoints = GetAbsVertices();
    unsigned int sz = convertedPoints.size();
    for (unsigned int i = 0; i < sz; i++)
    {
        convertedPoints[i] = convertedPoints[i]-newCenter;
    }
    return convertedPoints;
}
