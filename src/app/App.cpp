#include <sgw/App.hpp>
#include <sgw/AppData.hpp>
#include <app/GlutApp.hpp>
#include <app/GlfwApp.hpp>
#include <app/gApp.hpp>

sgw::IApp* sgw::App::Create(float windowWidth, float windowHeight)
{
    AppData appData;
    appData.windowSize.width = windowWidth;
    appData.windowSize.height = windowHeight;
    return Create(appData);
}

sgw::IApp* sgw::App::Create(const AppData& _appData)
{
    //IApp* pApp = new GlutApp();
    IApp* pApp = new GlfwApp();
    pApp = new GlfwApp();
    pApp->SetAppData(_appData);
    return pApp;
}
