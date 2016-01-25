#ifdef SGW_VIRTUAL_RENDERER_INTERFACE

#ifndef SGW_BASE_RENDERER_HPP_INCLUDED
#define SGW_BASE_RENDERER_HPP_INCLUDED

#include "FlushList.hpp"
#include "IRenderer.hpp"
#include <sgw/AppData.hpp>
//~ #include <math/transform.hpp>
//~ #include <math/camera.hpp>

namespace sgw
{

class BaseRenderer: public IRenderer
{    
public:
    BaseRenderer();
    virtual void Init(const AppData& appData) { m_appData = appData; }
protected:
    //~ Transform m_transform;
    //~ Camera m_camera;
    AppData m_appData;
    std::vector<FlushList> m_flushListCollection;
    void ClearFlushListCollection();
    void CheckFlushList(const BaseShape& shape);
};

}

#endif /* SGW_BASERENDERER_HPP_INCLUDED */ 

#endif
