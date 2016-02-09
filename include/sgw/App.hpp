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
     * 
     * @return Returns result of IsDone
     */
    inline bool MainLoop() 
    { 
        m_pIApp->ProcessEvents(); 
        m_pIApp->Render();
        return IsDone();
    }
    /**
     * \brief Get a const reference to the internal AppData object used by the App
     * 
     * @return the AppData object
     */    
    inline const AppData& GetAppData() const { return m_pIApp->GetAppData(); }
    /**
     * \brief Sets the function and parameters for color blending.
     * 
     * The resulting color is calculated depending on the
     * blending function and arguments chosen.
     * The func parameter may be one of:
     * 
     * BLENDER_FUNC_ADD: color = srcColor*srcScale + dstColor*dstScale 
     * 
     * BLENDER_FUNC_SRC_MINUS_DST: color = srcColor*srcScale - dstColor*dstScale 
     * 
     * BLENDER_FUNC_DST_MINUS_SRC: color = dstColor*dstScale - srcColor*srcScale 
     * 
     * where,
     * 
     * srcColor is the color of the pixel to be drawn.
     * 
     * dstColor is the color already present (background color).
     * 
     * srcScale/dstScale = 1 if src/dst = BLENDER_ONE
     * 
     * srcScale/dstScale = source alpha if src/dst = BLENDER_ALPHA
     * 
     * srcScale/dstScale = 1-source alpha if src/dst = BLENDER_INVERSE_ALPHA
     * 
     * @param func : Blending function. One of 
     * BLENDER_FUNC_ADD, 
     * BLENDER_FUNC_SRC_MINUS_DST or 
     * BLENDER_FUNC_DST_MINUS_SRC
     * @param src : Source color scale factor. Resulting color = 
     * source color * src + destination color * dst. 
     * One of LENDER_ONE, BLENDER_ALPHA 
     * or BLENDER_INVERSE_ALPHA
     * @param dst : Destination color scale factor. Resulting color = 
     * source color * src + destination color * dst. One of 
     * BLENDER_ONE, sgw::App::BLENDER_ALPHA or 
     * BLENDER_INVERSE_ALPHA
     */
    inline void SetBlender(int func, int src, int dst);
    
    /**
     * \brief Whether we should premultiply alpha on texture load
     */
    static bool PremultiplyAlpha;
    
    ///A constant used to set up blender
    static const int BLENDER_FUNC_ADD = 0;
    ///A constant used to set up blender
    static const int BLENDER_FUNC_SRC_MINUS_DST = 1;
    ///A constant used to set up blender
    static const int BLENDER_FUNC_DST_MINUS_SRC = 2;
    
    ///A constant used to set up blender
    static const int BLENDER_ONE = 0;
    ///A constant used to set up blender
    static const int BLENDER_ALPHA = 1;
    ///A constant used to set up blender
    static const int BLENDER_INVERSE_ALPHA = 2;
    
private:
    IApp* m_pIApp;
};

}

#endif
