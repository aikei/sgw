#ifndef SGW_APP_HPP_INCLUDED
#define SGW_APP_HPP_INCLUDED

#include "Prefs.hpp"
#include <app/IApp.hpp>

namespace sgw
{
    
struct AppData;
class BaseShape;

/**
 * Entry point to sgw and its main interface. It represents the window,
 * the renderer and all technical internals. The first thing you
 * need to create in order to use sgw.
 */
class App
{
public:
    /**
     * \brief Creates a new sgw App.
     * 
     * @param windowWidth : width of the window to be created
     * @param windowHeight : height of the window to be created
     * @param videoBackend : only used by the lib if it was compiled with SGW_RENDERER_VIRTUAL_INTERFACE, otherwise does nothing
     */
    App(float windowWidth, float windowHeight, 
        int videoBackend = SGW_CONST_RENDERER_BACKEND_OPEN_GL);
    /**
     * \brief Creates a new sgw App.
     * 
     * @param appData : AppData object containing info needed to create the App
     */
    App(const AppData& appData);
    /**
     * \brief Draws a shape onto the screen
     * 
     * @param shape : A shape to draw (including images)
     */
    void Draw(const BaseShape& shape);
    /**
     * \brief Whether internal 'done' flag of the App has been set.
     * 
     * The flag is set to false if you, for example, click the close button.
     * You can use it as a condition in your main cycle.
     */
    inline bool IsDone() const { return m_pIApp->IsDone(); }
    /**
     * \brief Processes all current events - part of main loop.
     * 
     * Should you wish to separate main loop into two stages, you need to
     * first call ProcessEvents() and then Render(). Don't call it if you 
     * already call MainLoop()
     */
    inline void ProcessEvents() { m_pIApp->ProcessEvents(); }
    /**
     * \brief Renders all drawn shapes onto the screen.
     * 
     * Should you wish to separate main loop into two stages, you need
     * to call Render() after ProcessEvents. 
     * Don't call it if you already call MainLoop()
     */    
    inline void Render() { m_pIApp->Render(); }
    /**
     * \brief Main Loop step
     * 
     * You need to run it in a loop until app ends.
     */
    inline void MainLoop() { m_pIApp->ProcessEvents(); m_pIApp->Render(); }
    /**
     * \brief Get a const reference to the internal AppData object used by the App
     * 
     * @return the AppData object
     */    
    inline const AppData& GetAppData() const { return m_pIApp->GetAppData(); }

protected:
    IApp* m_pIApp;
};

}

#endif
