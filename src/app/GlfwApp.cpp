#include "GlfwApp.hpp"

#include <sgw/Prefs.hpp>
#include <GLFW/glfw3.h>

#include <renderer/OpenGLRenderer.hpp>

GLFWwindow* window;


bool sgw::GlfwApp::Init(int backend)
{
    if (!glfwInit())
        throw "Glfw init failed";

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(m_appData.windowSize.width, m_appData.windowSize.height, m_appData.title.c_str(), NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return false;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    BaseApp::VideoBackend = backend;
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

void sgw::GlfwApp::Render()
{
    m_renderer->Render();
    glfwSwapBuffers(window);
}

void sgw::GlfwApp::ProcessEvents()
{
    glfwPollEvents();
    m_bDone = glfwWindowShouldClose(window);
}

