#ifndef SGW_TEXTURE_HPP_INCLUDED
#define SGW_TEXTURE_HPP_INCLUDED

namespace sgw
{

class Texture
{
friend class OpenGLRenderer;
public:
    Texture(const char* fileName);
    ~Texture();
    inline unsigned int GetWidth() const { return m_width; }
    inline unsigned int GetHeight() const { return m_height; }
    inline unsigned char* GetRawImageData() const { return m_pImageData; }
protected:
    unsigned int m_width, m_height;
    unsigned char* m_pImageData;
    void MakeConversions();
    void FileLoad(const char* fileName);
    void GPULoad();
    void GPUDelete();
    unsigned int glTextureObj;
};    
    
}

#endif
