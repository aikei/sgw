#ifdef SGW_VIRTUAL_RENDERER_INTERFACE

#include "BaseRenderer.hpp"
#include "FlushList.hpp"

#include <renderer/shape/BaseShape.hpp>

sgw::BaseRenderer::BaseRenderer()
{

}

void sgw::BaseRenderer::CheckFlushList(const BaseShape& shape)
{
    if (m_flushListCollection.empty() || 
        m_flushListCollection.back().type != shape.GetFlushListType())
    {
        FlushList flushList;
        flushList.type = shape.GetFlushListType();
        m_flushListCollection.push_back(flushList);
    }
}

void sgw::BaseRenderer::ClearFlushListCollection()
{
    m_flushListCollection.clear();
}

#endif
