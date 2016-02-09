#ifndef SGW_BASE_APP_INCLUDED
#define SGW_BASE_APP_INCLUDED

#include "IApp.hpp"
#include <sgw/AppData.hpp>
#include <renderer/shape/BaseShape.hpp>
#include <renderer/OpenGLRenderer.hpp>
    
namespace sgw
{
    
#ifdef SGW_VIRTUAL_RENDERER_INTERFACE
class IRenderer;
#elif SGW_RENDERER_OPENGL
class OpenGLRenderer;
#endif

class BaseApp
    :  public IApp
{
protected:
    AppData m_appData;
#ifdef SGW_VIRTUAL_RENDERER_INTERFACE
    IRenderer* m_renderer;
#elif defined SGW_RENDERER_OPENGL
    OpenGLRenderer* m_renderer;
#endif
    bool m_bDone;
public:
    BaseApp() { m_bDone = false; }
    inline bool IsDone() { return m_bDone; }
    inline void SetAppData(const AppData& _appData) { m_appData = _appData; }
    inline const AppData& GetAppData() const { return m_appData; }
    inline bool SetDone(bool done=true) { m_bDone = done; }   
    inline void Draw(const BaseShape& shape) { m_renderer->Draw(shape); }
    inline void SetBlender(int func, int src, int dst) { m_renderer->SetBlender(func,src,dst); }
    
    static int VideoBackend;    
};


}

#endif
