#ifndef SGW_RECT_INCLUDED
#define SGW_RECT_INCLUDED

#include "BaseShape.hpp"
#include <renderer/FlushList.hpp>

namespace sgw
{
    

class Rect : public BaseShape
{
public:

    Rect(float tlX, float tlY, float brX, float brY)
    {
        m_type = SHAPE_RECTANGLE;
        m_topLeft = Vec2(tlX,tlY);
        m_bottomRight = Vec2(brX,brY);
        m_size.width = brX-tlX;
        m_size.height = brY-tlY;
        m_flushListType = FlushList::TYPE_LINES;
    }
    
    Rect(const Vec2& _topLeft, const Vec2& _bottomRight)
    {
        m_type = SHAPE_RECTANGLE;
        m_topLeft = _topLeft;
        m_bottomRight = _bottomRight;
        m_size.width = _bottomRight.x-_topLeft.x;
        m_size.height = _bottomRight.y-_topLeft.y;
        m_flushListType = FlushList::TYPE_LINES;
    }
    
    inline const Vec2& GetTopLeft() const { return m_topLeft; }
    inline const Vec2& GetBottomRight() const { return m_bottomRight; }
    inline void SetFilled(bool filled) 
    { 
        BaseShape::SetFilled(filled);
        if (m_bFilled)
            m_flushListType = FlushList::TYPE_FILLED_TRIANGLES;
        else
            m_flushListType = FlushList::TYPE_LINES;
    }

    //~ virtual VerticesVector GetVertices();
    //~ virtual IndicesVector GetIndices();
protected:
    Vec2 m_topLeft, m_bottomRight;
};


}

#endif
