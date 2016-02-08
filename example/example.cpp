#include <sgw/sgw.hpp>
#include <cstdio>

int main(int argc, char** argv)
{   
    sgw::App app(800,600);
    
    sgw::Texture texture("../media/test.png");
    sgw::Texture texture2("../media/test2.png");
    sgw::Texture texture3("../media/test3.png");
    sgw::Image image(texture);
    image.SetPos(1.0f,1.0f);
    sgw::Image image2(texture2);
    image2.SetOpacity(0.75f);
    image2.SetSize(image2.GetSize()/2.0f);
    sgw::Image image3(texture3);
    image3.SetSize(image3.GetSize()/2.0f);
    image3.SetPos(300,200);
    //image3.SetColor(sgw::Color::RED);
    
    while(!app.IsDone())
    {
        sgw::Rect rect(1,1,100,100);
        rect.SetFilled(true);
        rect.SetColor(sgw::Color::YELLOW);
        app.Draw(rect);
        
        sgw::Rect rect2(150,150,200,200);
        //rect2.SetFilled(true);

        app.Draw(rect2);
        
        
        app.Draw(image);
        //app.Draw(image3);
        //app.Draw(image2);
        
        app.ProcessEvents();  
        app.Render();
    }

    return 0;
}
