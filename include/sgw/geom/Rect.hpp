#ifndef SGW_RECT_INCLUDED
#define SGW_RECT_INCLUDED

#include <renderer/shape/BaseShape.hpp>
#include <renderer/FlushList.hpp>

namespace sgw
{
    

class Rect : public BaseShape
{
public:

    Rect(float tlX, float tlY, float width, float height)
    {
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
    }
    
    inline void SetHeight(float newHeight)
    {
        m_size.height = newHeight;
    }
    
    inline void SetSize(const Size& newSize)
    {
        m_size = newSize;
    }

protected:
    inline void InitRect(const Vec2& topLeft, const Size& size)
    {
        m_type = SHAPE_RECTANGLE;
        m_pos = topLeft;
        m_size = size;
        m_flushListType = FlushList::TYPE_LINES;       
    }
};


}

#endif
