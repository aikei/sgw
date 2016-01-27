#include <sgw/sgw.hpp>


int main(int argc, char** argv)
{   
    sgw::App app(800,600);
    while(!app.IsDone())
    {
        sgw::Rect rect(1,1,100,100);
        rect.SetFilled(true);
        app.Draw(rect);
        
        sgw::Rect rect2(150,150,200,200);
        app.Draw(rect2);
        
        app.ProcessEvents();  
        app.Render();
    }
    
    return 0;
}
