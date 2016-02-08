#include "BaseApp.hpp"
#include <renderer/shape/BaseShape.hpp>
#include <renderer/OpenGLRenderer.hpp>

int sgw::BaseApp::VideoBackend = 0;

void sgw::BaseApp::Draw(const sgw::BaseShape& shape)
{
    m_renderer->Draw(shape);
}


