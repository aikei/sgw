#ifndef SGW_IRENDERER_INCLUDED
#define SGW_IRENDERER_INCLUDED

#ifdef SGW_VIRTUAL_RENDERER_INTERFACE

namespace sgw
{

class BaseShape;
class AppData;

class IRenderer
{
public:
    virtual void Init(const AppData& appData)=0;
    virtual void Render()=0;
    virtual void Draw(const BaseShape& shape)=0; 
};

}

#endif

#endif
