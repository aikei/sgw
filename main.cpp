#include <iostream>
#include <sgw/Prefs.hpp>
#include <sgw/AppData.hpp>
#include <sgw/App.hpp>
#include <utils/utils.hpp>
#include <math/math.hpp>
#include <renderer/shape/Rect.hpp>


int main(int argc, char** argv)
{   
    sgw::IApp* app = sgw::App::Create(800,600);
    app->Init();
    while(!app->IsDone())
    {
        app->ProcessEvents();
        
        sgw::Rect rect(sgw::Vec2(1,1),sgw::Vec2(100,100));
        rect.SetFilled(true);
        app->Draw(rect);
        
        sgw::Rect rect2(sgw::Vec2(150,150),sgw::Vec2(200,200));
        app->Draw(rect2);
          
        app->Render();
    }
    
    return 0;
}
