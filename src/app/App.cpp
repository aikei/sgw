#include <sgw/App.hpp>
#include <sgw/AppData.hpp>
#include <app/GlutApp.hpp>
#include <app/GlfwApp.hpp>

sgw::App::App(const AppData& appData)
{
    m_pIApp = new GlfwApp();
    m_pIApp->SetAppData(appData);
    m_pIApp->Init(appData.videoBackend);
}

sgw::App::App(float windowWidth, float windowHeight, int videoBackend)
{
    AppData appData;
    appData.windowSize.width = windowWidth;
    appData.windowSize.height = windowHeight;
    appData.videoBackend = videoBackend;
    m_pIApp = new GlfwApp();
    m_pIApp->SetAppData(appData);
    m_pIApp->Init(appData.videoBackend);      
}

void sgw::App::Draw(const BaseShape& shape)
{
    m_pIApp->Draw(shape);
}
