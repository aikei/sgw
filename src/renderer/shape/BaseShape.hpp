#ifndef SGW_BASE_SHAPE_INCLUDED
#define SGW_BASE_SHAPE_INCLUDED

#include <math/math.hpp>
#include <sgw/Color.hpp>
#include <renderer/FlushList.hpp>
#include <sgw/geom/Vec2.hpp>

namespace sgw
{

class Texture;

class BaseShape
{
public:

    enum Shape
    {
        SHAPE_RECTANGLE = 0,
        SHAPE_IMAGE = 1
    };
    
    BaseShape()
    {
        m_outlineThickness = 1.0f;
        m_bFilled = false;
        m_opacity = 1.0f;
    }   
    
    inline void SetColor(const Color& color) 
    { 
        m_color = color; 
        SetOpacity(m_opacity);
    } 
    inline const Color& GetColor() const { return m_color; }
    inline void SetOutlineThickness(float thickness) { m_outlineThickness = thickness; }
    inline float GetOutlineThickness() const { return m_outlineThickness; }
    inline const Size& GetSize() const { return m_size; }
    inline virtual void SetFilled(bool filled) { m_bFilled = filled; }
    inline bool IsFilled() const { return m_bFilled; }
    inline int GetType() const { return m_type; }
    inline const Vec2& GetPos() const { return m_pos; }
    inline void SetPos(const Vec2& pos) { m_pos = pos; }
    inline void SetPos(float x, float y) { m_pos.x = x; m_pos.y = y; }
    inline int GetFlushListType() const { return m_flushListType; }
    inline virtual const Texture* GetTexture() const { return NULL; }
    inline void SetOpacity(float opacity) 
    { 
        m_opacity = opacity; 
        m_color.a = static_cast<unsigned char>(m_opacity*(sgw::BITS_PER_CHANNEL-1)); 
    }
protected:
    float m_opacity;
    int m_flushListType;
    int m_type;
    Size m_size;
    Vec2 m_pos;
    Color m_color;
    float m_outlineThickness;
    bool m_bFilled;
};


}

#endif
