#include "transform.hpp"
#include "camera.hpp"

sgw::Transform::Transform()
{
    m_rotation.MakeIdentity();
    m_scale.MakeIdentity();
    m_translation.MakeIdentity();
    m_cameraRotation.MakeIdentity();
    m_cameraTranslation.MakeIdentity();
    m_perspectiveProjection.MakeIdentity();
}

void sgw::Transform::SetRotation(float rotationX, float rotationY, float rotationZ)
{
    m_rotation.MakeIdentity();
    m_rotation.SetRotation(rotationX, rotationY, rotationZ);
}

void sgw::Transform::SetScale(float scaleX, float scaleY, float scaleZ)
{
    m_scale.MakeIdentity();
    m_scale.SetScale(scaleX, scaleY, scaleZ);
}

void sgw::Transform::SetTranslation(float x, float y, float z)
{
    m_translation.MakeIdentity();   
    m_translation.SetTranslation(x, y, z);
}

void sgw::Transform::SetPerspectiveProjection(float w, float h, float fov, float zNear, float zFar)
{
    m_perspectiveProjection.SetPerspectiveProjection(w,h,fov,zNear,zFar);
}

void sgw::Transform::SetOrthographicProjection(float w, float h, float zNear, float zFar)
{
    m_perspectiveProjection.SetOpenGLOrthographicProjection(w,h,zNear,zFar);
}

//~ const Mat4& Pipeline::GetWorldTransform()
//~ {
    //~ m_worldTransform = m_translation*m_rotation*m_scale;
    //~ return m_worldTransform;
//~ }
//~ 
//~ const Mat4& Pipeline::GetProjectedWorldTransform()
//~ {
    //~ m_worldTransform = m_translation*m_rotation*m_scale;
    //~ m_worldTransform = m_perspectiveProjection*m_worldTransform;
    //~ return m_worldTransform;
//~ }

const sgw::Mat4& sgw::Transform::GetWorldTransform()
{
    m_worldTransform = m_perspectiveProjection*m_cameraRotation*m_cameraTranslation*m_translation*m_rotation*m_scale;
    return m_worldTransform;
}

void sgw::Transform::SetCameraRotation(const Vec3& target, const Vec3& upAxis)
{
    m_cameraRotation.SetCameraTransform(target,upAxis);
}

void sgw::Transform::SetCameraRotation(const Camera& camera)
{
    SetCameraRotation(camera.GetTarget(),camera.GetUpAxis());
}

void sgw::Transform::SetCameraPosition(const Vec3& pos)
{
    m_cameraTranslation.SetTranslation(-pos.x,-pos.y,-pos.z);
}

void sgw::Transform::SetCameraPosition(const Camera& camera)
{
    SetCameraPosition(camera.GetPos());
}

void sgw::Transform::SetCamera(const Camera& camera)
{
    SetCameraRotation(camera.GetTarget(),camera.GetUpAxis());
    SetCameraPosition(camera.GetPos());
}
