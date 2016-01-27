#ifndef SGW_APP_HPP_INCLUDED
#define SGW_APP_HPP_INCLUDED

#include "Prefs.hpp"
#include <app/IApp.hpp>

namespace sgw
{
    
struct AppData;
class BaseShape;

class App
{
public:
    App(float windowWidth, float windowHeight, int videoBackend = SGW_CONST_RENDERER_BACKEND_OPEN_GL);
    App(const AppData& appData);
    void Draw(const BaseShape& shape);
    inline bool IsDone() const { return m_pIApp->IsDone(); }
    inline void ProcessEvents() { m_pIApp->ProcessEvents(); }
    inline void Render() { m_pIApp->Render(); }
    inline const AppData& GetAppData() const { return m_pIApp->GetAppData(); }
protected:
    IApp* m_pIApp;
};

}

#endif
