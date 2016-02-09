#ifndef SGW_IMAGE_HPP_INCLUDED
#define SGW_IMAGE_HPP_INCLUDED

#include "Rect.hpp"

namespace sgw
{

class Texture;

class Image : public Rect
{
public:
    Image(const Texture& texture);
    inline const Texture* GetTexture() const { return &m_texture; }
    inline void SetFilled(bool filled) {  }
protected:
    const Texture& m_texture;
};

}

#endif
