#include <GL/freeglut.h>

#include <sgw/Prefs.hpp>

#include "GlutApp.hpp"
#include <renderer/OpenGLRenderer.hpp>
#include <sgw/geom/Rect.hpp>

static sgw::IApp* pApp;

static void OnClose()
{
    pApp->SetDone();
}

static void StaticRender()
{
    pApp->Render();
}   


bool sgw::GlutApp::Init(int backend)
{
    pApp = this;
    BaseApp::VideoBackend = backend;
    glutInit(&m_appData.argc, m_appData.argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(m_appData.windowPosition.x,m_appData.windowPosition.y);
    glutInitWindowSize(m_appData.windowSize.width, m_appData.windowSize.height);
    if (!glutGet(GLUT_DISPLAY_MODE_POSSIBLE))
    {
        exit(1);
    }   
    glutCreateWindow(m_appData.title.c_str());
    
    glutDisplayFunc(StaticRender);
    glutIdleFunc(StaticRender);
    glutCloseFunc(OnClose);
    glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);

#ifdef SGW_VIRTUAL_RENDERER_INTERFACE
    switch(backend)
    {
        case SGW_CONST_RENDERER_BACKEND_OPEN_GL:
            m_renderer = new OpenGLRenderer();
            break;
    }    
#elif defined SGW_RENDERER_OPENGL  
    m_renderer = new OpenGLRenderer();
#endif

    m_renderer->Init(m_appData);
    
    return true;
}

void sgw::GlutApp::Render()
{
    m_renderer->Render();
    glutSwapBuffers();
}

void sgw::GlutApp::ProcessEvents()
{
    glutMainLoopEvent();
}

void sgw::GlutApp::Exit()
{
    glutLeaveMainLoop();
}

