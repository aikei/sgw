#ifndef SGW_RECT_INCLUDED
#define SGW_RECT_INCLUDED

#include <renderer/shape/BaseShape.hpp>
#include <renderer/FlushList.hpp>
#include <utils/Logger.hpp>

namespace sgw
{
    

class Rect : public BaseShape
{
public:

    Rect(float tlX, float tlY, float width, float height)
    {
        m_textureType = TextureType::TEXTURE_TYPE_QUAD;
        Logger::FLogInfo("Constructing a rect with tlX: %f, tly: %f, width: %f, height: %f",tlX,tlY,width,height);
        m_type = SHAPE_RECTANGLE;
        m_flushListType = FlushList::TYPE_LINES;
        InitRect(Vec2(tlX, tlY), Size(width, height));
    }
    
    Rect(const Vec2& topLeft, const Size& size)
    {
        InitRect(topLeft, size);
    }    
    
    inline void SetFilled(bool filled) 
    { 
        BaseShape::SetFilled(filled);
        if (m_bFilled)
            m_flushListType = FlushList::TYPE_FILLED_TRIANGLES;
        else
            m_flushListType = FlushList::TYPE_LINES;
    }
    
    inline void SetWidth(float newWidth)
    {
        m_size.width = newWidth;
        InitRect(m_pos, m_size);
    }
    
    inline void SetHeight(float newHeight)
    {
        m_size.height = newHeight;
        InitRect(m_pos, m_size);
    }
    
    inline void SetSize(const Size3D& newSize)
    {
        m_size = newSize;
        InitRect(m_pos, m_size);
    }
    
    inline void SetPos(const Vec3& pos)
    {
        m_pos = pos;
        InitRect(m_pos, m_size);
    }
    
    inline void SetPos(float x, float y, float z=0.0f)
    {
        SetPos(Vec3(x,y,z));
    }
    
protected:
    inline void InitRect(const Vec3& topLeft, const Size3D& size)
    {
        Logger::FLogInfo("Rect topLeft: %f, %f, %f",topLeft.x,topLeft.y,topLeft.z);
        Logger::FLogInfo("Rect size: %f, %f, %f",size.width,size.height,size.depth);
        Logger::FLogInfo("type: %d",m_type);
        m_pos = topLeft;
        SetCenter(topLeft+size/2.0f);
        m_size = size;
        m_points.clear();
        m_points.emplace_back(Vec3(-size.width/2.0f, -size.height/2.0f),Vec2(0.0f,1.0f));
        m_points.emplace_back(Vec3(size.width/2.0f, -size.height/2.0f),Vec2(1.0f,1.0f));
        m_points.emplace_back(Vec3(size.width/2.0f, size.height/2.0f),Vec2(1.0f,0.0f));
        m_points.emplace_back(Vec3(-size.width/2.0f, size.height/2.0f),Vec2(0.0f,0.0f));
        Logger::FLogInfo("Points: ");
        Logger::FLog(Logger::LOG_LEVEL_INFO,m_points);
        Logger::FLogInfo("Rect center: %f, %f, %f",m_center.x,m_center.y,m_center.z);
    }
};


}

#endif
