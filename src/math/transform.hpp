#ifndef SGW_TRANSFORM_HPP_INCLUDED
#define SGW_TRANSFORM_HPP_INCLUDED

#include <math/math.hpp>

namespace sgw
{


class Camera;

class Transform
{
public:
	Transform();
	const Mat4& GetWorldTransform();
	//const Mat4& GetProjectedWorldTransform();
	void SetRotation(float rotationX, float rotationY, float rotationZ);
	void SetScale(float scaleX, float scaleY, float scaleZ);
	void SetTranslation(float x, float y, float z);
	void SetPerspectiveProjection(float w, float h, float fov=PI/4, float zNear=0.0f,float zFar=100.0f);
	void SetOrthographicProjection(float w, float h, float zNear=0.0f, float zFar=100.0f);
	void SetCameraPosition(const Vec3& pos);
	void SetCameraRotation(const Vec3& target, const Vec3& upAxis);
	void SetCameraPosition(const Camera& camera);
	void SetCameraRotation(const Camera& camera);	
	void SetCamera(const Camera& camera);
private: 
	Mat4 m_worldTransform;

	Mat4 m_rotation;
	Mat4 m_scale;
	Mat4 m_translation;
	Mat4 m_perspectiveProjection;
	Mat4 m_cameraRotation;
	Mat4 m_cameraTranslation;
};


}

#endif
