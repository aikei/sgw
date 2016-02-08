#include <utils/Logger.hpp>
#include <sgw/geom/Size.hpp>
#include <sgw/geom/Image.hpp>
#include <sgw/Texture.hpp>

sgw::Image::Image(const Texture& texture)
    : m_texture(texture), Rect(Vec2(0,0),Size(texture.GetWidth(),texture.GetHeight()))
{
    m_type = sgw::BaseShape::SHAPE_IMAGE;
    m_bFilled = true;
    m_flushListType = FlushList::TYPE_FILLED_TRIANGLES;
}
