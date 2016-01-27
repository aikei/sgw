#ifndef __CAMERA_H_INCLUDED__
#define __CAMERA_H_INCLUDED__

#include <math/math.hpp>
#include <sgw/geom/Size.hpp>

namespace sgw
{
    
struct AppData;

class Camera
{
public:
    Camera(const AppData& appData);
    Camera(float screenWidth, float screenHeight);
    Camera(float screenWidth, float screenHeight, const Vec3& pos, const Vec3& target, const Vec3& upAxis);
    void Init();
    void OnRender();
    void Update();
    const Vec3& GetPos() const;
    const Vec3& GetTarget() const;
    const Vec3& GetUpAxis() const;

private:
    void Construct(float screenWidth, float screenHeight);
    bool m_onUpperEdge, m_onLowerEdge, m_onLeftEdge, m_onRightEdge;
    float m_angleH,m_angleV;
    Size m_screenSize;
    Vec2 m_mousePos;
    
    Vec3 m_pos;
    Vec3 m_target;
    Vec3 m_upAxis;    
};


}

#endif
