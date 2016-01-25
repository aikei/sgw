#ifndef TEXTURE_H
#define	TEXTURE_H

#include <string>
#include <GL/glew.h>
//#include "/usr/include/ImageMagick/Magick++.h"
#include <Magick++.h>

class Texture
{
public:
   Texture(GLenum textureTarget, const std::string& fileName);
   bool Load();
   void Bind(GLenum TextureUnit);
   
private:
    std::string m_fileName;
    GLenum m_textureTarget;
    GLuint m_textureObj;
    Magick::Image m_image;
    Magick::Blob m_blob;   
};

#endif
