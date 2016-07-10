#include <sgw/geom/Sphere.hpp>
#include <utils/Logger.hpp>
#include <renderer/Vertex.hpp>
#include <cmath>
#include <cstdlib>

void sgw::Sphere::InitSphere(const Vec3& center, float radius, unsigned int splits)
{
    sgw::Logger::FLogTrace("Sphere::InitSphere");
    m_textureType = TextureType::TEXTURE_TYPE_SPHERE;
    m_radius = radius;
    for (unsigned int i = 0; i < splits; i++)
    {
        std::vector<Triangle> triangles;
        unsigned int sz = m_triangles.size();
        for (unsigned int j = 0; j < sz; j++)
        {
            std::vector<Triangle> splitTriangles = m_triangles[j].Split();
            triangles.insert(triangles.end(), splitTriangles.begin(), splitTriangles.end());
        }
        m_triangles.clear();
        m_triangles.insert(m_triangles.end(), triangles.begin(), triangles.end());
        //m_triangles = triangles;
    }
    
    unsigned int sz = m_triangles.size();
    m_points.clear();
    //m_points.reserve(sz*3);
    for (unsigned int i = 0; i < sz; i++)
    {
        std::vector<Vertex> trianglePoints = m_triangles[i].GetConvertedVertices(m_center);
        std::vector <size_t> edgePointIndices;
        float edgeType = 0.0f;
        size_t thirdPointIndex;
        for (size_t j = 0; j < 3; j++)
        {
            trianglePoints[j].Normalize();
            
            trianglePoints[j].texCoord.x = (atan2(trianglePoints[j].pos.z, trianglePoints[j].pos.x)/(2.0f*sgw::PI))+0.625f;
            
            if (trianglePoints[j].texCoord.x > 1.0f)
                trianglePoints[j].texCoord.x -= 1.0f;
            trianglePoints[j].texCoord.y = 0.5f-asin(trianglePoints[j].pos.y)/sgw::PI;
            trianglePoints[j] = trianglePoints[j]*radius;
            if (trianglePoints[j].texCoord.x == 1.0f) 
            {
                edgePointIndices.push_back(j);
                edgeType = 1.0f;
            }
            else if (trianglePoints[j].texCoord.x == 0.0f)
            {
                edgePointIndices.push_back(j);
                edgeType = 0.0f;
            }
            else
            {
                thirdPointIndex = j;
            }
        }
        if (edgePointIndices.size() > 0)
        {
            if (edgeType == 1.0f)
            {
                if (trianglePoints[thirdPointIndex].texCoord.x < 0.5f)
                {
                    for (size_t j = 0; j < 3; j++)
                    {
                        if (trianglePoints[j].texCoord.x == 1.0f)
                            trianglePoints[j].texCoord.x = 0.0f;
                    }
                }
            }
            else if (edgeType == 0.0f)
            {
                if (trianglePoints[thirdPointIndex].texCoord.x > 0.5f)
                {
                    for (size_t j = 0; j < 3; j++)
                    {
                        if (trianglePoints[j].texCoord.x == 0.0f)
                            trianglePoints[j].texCoord.x = 1.0f;
                    }
                }                
            }
        }
            
        m_points.insert(m_points.end(), trianglePoints.begin(), trianglePoints.end());
    }
    
    sz = m_points.size();
    //sgw::Logger::FLogInfo("Total points: %d",sz);

    sgw::Logger::FLogInfo("======== Sphere coords ========");
    sgw::Logger::FLog(Logger::LOG_LEVEL_INFO, m_points);
    SetFilled(true);
}

/*
void sgw::Sphere::InitSphere(const Vec3& center, float radius, unsigned int splits)
{
    sgw::Logger::FLogTrace("Sphere::InitSphere");
    m_textureType = TextureType::TEXTURE_TYPE_SPHERE;
    m_radius = radius;
    for (unsigned int i = 0; i < splits; i++)
    {
        std::vector<Triangle> triangles;
        unsigned int sz = m_triangles.size();
        for (unsigned int j = 0; j < sz; j++)
        {
            std::vector<Triangle> splitTriangles = m_triangles[j].Split();
            triangles.insert(triangles.end(), splitTriangles.begin(), splitTriangles.end());
        }
        m_triangles.clear();
        m_triangles.insert(m_triangles.end(), triangles.begin(), triangles.end());
        //m_triangles = triangles;
    }
    
    unsigned int sz = m_triangles.size();
    m_points.clear();
    //m_points.reserve(sz*3);
    for (unsigned int i = 0; i < sz; i++)
    {
        std::vector<Vertex> trianglePoints = m_triangles[i].GetConvertedVertices(m_center);
        //sgw::Logger::FLogDebug("triangle %d points",i);
        //sgw::Logger::FLogDebug(trianglePoints);        
        m_points.insert(m_points.end(), trianglePoints.begin(), trianglePoints.end());
    }
    
    sz = m_points.size();
    //sgw::Logger::FLogInfo("Total points: %d",sz);
    float r = m_points[0].pos.GetLength();
    unsigned int triangleIndex = -1;
    for (unsigned int i = 0; i < sz; i++)
    {
        m_points[i].Normalize();
        
        m_points[i].texCoord.x = (atan2(m_points[i].pos.z, m_points[i].pos.x)/(2.0f*sgw::PI))+0.625f;
        
        if (m_points[i].texCoord.x > 1.0f)
            m_points[i].texCoord.x -= 1.0f;
        m_points[i].texCoord.y = 0.5f-asin(m_points[i].pos.y)/sgw::PI;

        //sgw::Logger::FLogInfo("m_points[%d]");
        //sgw::Logger::FLog(Logger::LOG_LEVEL_INFO,m_points[i]);
        m_points[i] = m_points[i]*radius;
    }
    sgw::Logger::FLogInfo("======== Sphere coords ========");
    sgw::Logger::FLog(Logger::LOG_LEVEL_INFO, m_points);
    SetFilled(true);
}
*/

/*
void sgw::Sphere::InitSphere(const Vec3& center, float radius, unsigned int splits)
{
    sgw::Logger::FLogInfo("----Inside InitSphere");
    m_textureType = TextureType::TEXTURE_TYPE_SPHERE;
    m_radius = radius;
    for (unsigned int i = 0; i < splits; i++)
    {
        std::vector<Triangle> triangles;
        unsigned int sz = m_triangles.size();
        for (unsigned int j = 0; j < sz; j++)
        {
            std::vector<Triangle> splitTriangles = m_triangles[j].Split();
            triangles.insert(triangles.begin(), splitTriangles.begin(), splitTriangles.end());
        }
        m_triangles.clear();
        m_triangles.insert(m_triangles.end(), triangles.begin(), triangles.end());
        //m_triangles = triangles;
    }
    
    unsigned int sz = m_triangles.size();
    m_points.clear();
    //m_points.reserve(sz*3);
    for (unsigned int i = 0; i < sz; i++)
    {
        std::vector<Vec3> trianglePoints = m_triangles[i].GetConvertedPoints(m_center);
        //sgw::Logger::FLogInfo("triangle %d points",i);
        //sgw::Logger::FLogInfo(trianglePoints);        
        m_points.insert(m_points.end(), trianglePoints.begin(), trianglePoints.end());
    }
    
    sz = m_points.size();
    //sgw::Logger::FLogInfo("Total points: %d",sz);
    m_geoCoordinates.reserve(sz);
    float r = m_points[0].GetLength();
    for (unsigned int i = 0; i < sz; i++)
    {
        m_points[i] = m_points[i].Normalize();
        m_geoCoordinates.emplace_back(0.0f,0.0f,0.0f);
        
        m_geoCoordinates[i].x = (atan2(m_points[i].z, m_points[i].x)/(2.0f*sgw::PI))+0.625f;
        if (m_geoCoordinates[i].x > 1.0f)
            m_geoCoordinates[i].x -= 1.0f;
        m_geoCoordinates[i].y = 0.5f-asin(m_points[i].y)/sgw::PI;

        
        m_points[i] = m_points[i]*radius;
        //m_geoCoordinates[i].y = 1.0f-m_geoCoordinates[i].y;
    }
    //sgw::Logger::FLog(Logger::LOG_LEVEL_ERROR, m_geoCoordinates);
    SetFilled(true);
}
*/
