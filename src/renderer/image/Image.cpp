#include <utils/Logger.hpp>
#include <sgw/geom/Size.hpp>
#include <sgw/geom/Image.hpp>
#include <sgw/Texture.hpp>

sgw::Image::Image(std::shared_ptr<Texture> texture)
    : Rect(Vec2(0,0),Size(texture->GetWidth(),texture->GetHeight()))
{
    m_spTexture = texture;
    m_textureType = TextureType::TEXTURE_TYPE_QUAD;
    m_type = sgw::BaseShape::SHAPE_IMAGE;
    m_bFilled = true;
    m_flushListType = FlushList::TYPE_FILLED_TRIANGLES;
}
