#ifndef SGW_OPENGL_RENDERER_HPP_INCLUDED
#define SGW_OPENGL_RENDERER_HPP_INCLUDED

#include "BaseRenderer.hpp"

#include "FlushList.hpp"
#include <sgw/AppData.hpp>

#ifndef SGW_VIRTUAL_RENDERER_INTERFACE
#include "shape/BaseShape.hpp"
#endif


namespace sgw
{
class Texture;
class OpenGLRenderer
#ifdef SGW_VIRTUAL_RENDERER_INTERFACE
    : public BaseRenderer
#endif
{
public:
    OpenGLRenderer();
    void Init(const AppData& appData);
    void Render();
    void Draw(const BaseShape& shape);
    
#ifndef SGW_VIRTUAL_RENDERER_INTERFACE
protected:
    AppData m_appData;
    std::vector<FlushList> m_flushListCollection;
    void ClearFlushListCollection();
    void CheckFlushList(const BaseShape& shape);
    void UnbindTexture();
    void BindTexture(unsigned int textureUnit, const Texture& texture);
#endif
};


}

#endif /* OPENGLRENDERER_H */ 
