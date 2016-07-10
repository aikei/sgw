#ifndef SGW_IMAGE_HPP_INCLUDED
#define SGW_IMAGE_HPP_INCLUDED

#include "Rect.hpp"

namespace sgw
{

class Texture;

class Image : public Rect
{
public:
    Image(std::shared_ptr<Texture> texture);
    inline void SetFilled(bool filled) {  }
};

}

#endif
