#ifndef SGW_BASE_SHAPE_INCLUDED
#define SGW_BASE_SHAPE_INCLUDED

#include <math/math.hpp>
#include <math/Transform.hpp>
#include <sgw/Color.hpp>
#include <renderer/FlushList.hpp>
#include <renderer/Vertex.hpp>
#include <sgw/geom/Vec3.hpp>
#include <sgw/geom/Size3D.hpp>
#include <vector>
#include <memory>

namespace sgw
{

class Texture;

class BaseShape
{
public:

    enum Shape
    {
        SHAPE_RECTANGLE = 0,
        SHAPE_IMAGE = 1,
        SHAPE_TRIANGLE = 2,
        SHAPE_OCTAHEDRON  = 3,
        SHAPE_SPHERE = 4
    };
    
    enum TextureType
    {
        TEXTURE_TYPE_NONE = 0,
        TEXTURE_TYPE_QUAD = 1,
        TEXTURE_TYPE_SPHERE = 2
    };
    
    BaseShape()
    {
        m_outlineThickness = 1.0f;
        m_bFilled = false;
        m_opacity = 1.0f;
        m_textureType = TEXTURE_TYPE_NONE;
    }   
    
    inline void SetColor(const Color& color) 
    { 
        m_color = color; 
        SetOpacity(m_opacity);
    } 
    inline const Color& GetColor() const { return m_color; }
    inline void SetOutlineThickness(float thickness) { m_outlineThickness = thickness; }
    inline float GetOutlineThickness() const { return m_outlineThickness; }
    inline const Size3D& GetSize() const { return m_size; }
    inline virtual void SetFilled(bool filled) { m_bFilled = filled; }
    inline bool IsFilled() const { return m_bFilled; }
    inline int GetType() const { return m_type; }
    inline const Vec3& GetPos() const { return m_pos; }
    virtual inline void SetPos(const Vec3& pos) { m_pos = pos; }
    virtual inline void SetPos(float x, float y, float z=0.0f) { SetPos(Vec3(x,y,z)); }
    inline const Vec3& GetCenter() const { return m_center; }
    virtual inline void SetCenter(const Vec3& center) { m_center = center; m_transform.SetTranslation(m_center); }
    inline void SetCenter(float x, float y, float z=0.0f) { SetCenter(Vec3(x,y,z)); }    
    inline int GetFlushListType() const { return m_flushListType; }
    inline virtual const Texture* GetTexture() const { if (m_spTexture) return m_spTexture.get(); return NULL; }
    inline void SetTexture(std::shared_ptr<Texture> texture) { m_spTexture = texture; }
    inline bool GetDrawOutline() const { return m_bDrawOutline; }
    inline bool SetDrawOutline(bool drawOutline) { m_bDrawOutline = drawOutline; }
    
    const std::vector<Vertex>& GetVertices() const { return m_points; }
    std::vector<Vertex> GetAbsVertices() const;
    std::vector<Vertex> GetConvertedVertices(const Vec3& newCenter) const;
    
    inline void SetOpacity(float opacity) 
    { 
        m_opacity = opacity; 
        m_color.a = static_cast<unsigned char>(m_opacity*(sgw::BITS_PER_CHANNEL-1)); 
    }
    inline const Transform& GetTransform() const { return m_transform; }
    inline void Rotate(float angleX, float angleY, float angleZ) { m_transform.SetRotation(angleX, angleY, angleZ); }
    inline int GetTextureType() const { return m_textureType; }
protected:
    Transform m_transform;
    float m_opacity;
    std::shared_ptr<Texture> m_spTexture;
    std::vector<Vertex> m_points;
    int m_flushListType, m_type, m_textureType;
    Size3D m_size;
    Vec3 m_pos, m_center;
    Color m_color;
    float m_outlineThickness;
    bool m_bFilled, m_bDrawOutline;
};


}

#endif
