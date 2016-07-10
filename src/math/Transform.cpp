#include "Transform.hpp"
#include <utils/Logger.hpp>
#include <renderer/Vertex.hpp>

sgw::Transform::Transform()
{
    m_rotation.MakeIdentity();
    m_scale.MakeIdentity();
    m_translation.MakeIdentity();
    MakeFullTransform();
}

void sgw::Transform::SetRotation(float rotationX, float rotationY, float rotationZ)
{
    m_rotation.MakeIdentity();
    m_rotation.SetRotation(rotationX, rotationY, rotationZ);
    MakeFullTransform();
}

void sgw::Transform::SetScale(float scaleX, float scaleY, float scaleZ)
{
    m_scale.MakeIdentity();
    m_scale.SetScale(scaleX, scaleY, scaleZ);
    MakeFullTransform();
}

void sgw::Transform::SetTranslation(const Vec3& translation)
{
    SetTranslation(translation.x, translation.y, translation.z);
}

void sgw::Transform::SetTranslation(float x, float y, float z)
{
    m_translation.MakeIdentity();   
    m_translation.SetTranslation(x, y, z);
    MakeFullTransform();
}

void sgw::Transform::MakeFullTransform()
{
    m_fullTransform = m_translation*m_rotation*m_scale;
}

sgw::Vec3 sgw::Transform::Apply(const Vec3& v) const
{
    return m_fullTransform*v;
}

std::vector<sgw::Vertex> sgw::Transform::Apply(const std::vector<Vertex>& vertices) const
{
    Logger::FLogTrace("----In Transform::Apply, full transform: ");
    Logger::FLog(Logger::LOG_LEVEL_TRACE,m_fullTransform);
    unsigned int sz = vertices.size();
    std::vector<sgw::Vertex> transformedVertices;
    transformedVertices.reserve(sz);
    Logger::FLogTrace("vertices before transform: ");
    Logger::FLog(Logger::LOG_LEVEL_TRACE,vertices);
    for (unsigned int i = 0; i < sz; i++)
    {
        transformedVertices.emplace_back(m_fullTransform*vertices[i].pos,vertices[i].texCoord);
    }
    Logger::FLogTrace("vertices after transform: ");
    Logger::FLog(Logger::LOG_LEVEL_TRACE,transformedVertices);    
    return transformedVertices;
}
