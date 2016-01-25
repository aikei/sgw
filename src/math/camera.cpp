#include "camera.hpp"
#include <math/math.hpp>
#include <utils/utils.hpp>

#include <math.h>
#include <GL/glut.h>
#include <sgw/AppData.hpp>

static const float STEP = 1.0f;
const static float EDGE_STEP = 0.0001;
//const static float MOUSE_STEP = PI/90;
const static int MARGIN = 30;

void sgw::Camera::Construct(float screenWidth, float screenHeight)
{
	m_pos = Vec3(0.0f, 0.0f, 0.0f);
	m_target = Vec3(0.0f, 0.0f, 1.0f);
	m_upAxis = Vec3(0.0f, 1.0f, 0.0f);
	m_screenSize = Size(screenWidth, screenHeight);	
}

sgw::Camera::Camera(const AppData& appData)
{
	Construct(appData.windowSize.width,appData.windowSize.height);
}

sgw::Camera::Camera(float screenWidth, float screenHeight)
{
	Construct(screenWidth,screenHeight);
}

sgw::Camera::Camera(float screenWidth, float screenHeight, const Vec3& pos, const Vec3& target, const Vec3& upAxis)
{
	m_pos = pos;
	m_target = target;
	m_target.Normalize();
	m_upAxis = upAxis;
	m_upAxis.Normalize();
	m_screenSize = Size(screenWidth, screenHeight);
}

void sgw::Camera::OnRender()
{

}

void sgw::Camera::Update()
{
    const Vec3 vAxis(0.0f, 1.0f, 0.0f);

    // Rotate the view vector by the horizontal angle around the vertical axis
    Vec3 view(1.0f, 0.0f, 0.0f);
    view.Rotate(m_angleH, vAxis);
    view.Normalize();

    // Rotate the view vector by the vertical angle around the horizontal axis
    Vec3 hAxis = vAxis.Cross(view);
    hAxis.Normalize();
    view.Rotate(m_angleV, hAxis);
    view.Normalize();

    m_target = view;
    m_target.Normalize();

    m_upAxis = m_target.Cross(hAxis);
    m_upAxis.Normalize();
} 

const sgw::Vec3& sgw::Camera::GetPos() const
{
	return m_pos;
}

const sgw::Vec3& sgw::Camera::GetTarget() const
{
	return m_target;
}

const sgw::Vec3& sgw::Camera::GetUpAxis() const
{
	return m_upAxis;
}
