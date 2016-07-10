#include <sgw/sgw.hpp>
#include <cstdio>
#include <vector>
#include <cmath>
#include <memory>
#include <utils/Logger.hpp>

int main(int argc, char** argv)
{   
    sgw::App app(800,600);
    
    std::shared_ptr<sgw::Texture> texture = std::make_shared<sgw::Texture>("../media/test.png");
    std::shared_ptr<sgw::Texture> texture2 = std::make_shared<sgw::Texture>("../media/test2.png");
    std::shared_ptr<sgw::Texture> texture3 = std::make_shared<sgw::Texture>("../media/test3.png");
    std::shared_ptr<sgw::Texture> earthMap = std::make_shared<sgw::Texture>("../media/earthMap2.png");
    
    sgw::Image image(texture);
    image.SetPos(1.0f,1.0f);
    
    sgw::Image image2(texture2);
    image2.SetOpacity(0.75f);
    image2.SetSize(image2.GetSize()/2.0f);
    
    sgw::Image image3(earthMap);
    image3.SetSize(image3.GetSize()/2.0f);
    image3.SetPos(300.0f,200.0f);

    sgw::Rect rect(1,1,100,100);
    //rect.SetFilled(true);
    //rect.SetColor(sgw::Color::YELLOW);
    
    //~ sgw::Rect rect2(150,150,200,200);
    //~ rect2.SetFilled(false);
    //~ rect2.Rotate(0.0f,sgw::PI/4,0.0f);
    
    sgw::Triangle tr(250,250,275,300,300,250);
    tr.SetFilled(true);
    
    std::vector<sgw::Triangle> trs = tr.Split();
    
    sgw::Octahedron oc(400,300,0,100,0.8660254*100,100);
    oc.SetFilled(true);
    float rot = 0.0f;
    float rad = 300.0f;
    sgw::Sphere sph(400.0f,300.0f,0.0f,rad,3);
    //sph.SetFilled(true);
    sph.SetOpacity(1.0f);
    sph.SetTexture(earthMap);
    //~ sgw::Triangle tr2(275,250,0,287,275,0,137.5,150,0);
    //~ tr2.SetFilled(false);    
    
    sgw::Sphere sph2(400.0f,300.0f,0.0f,rad+1.0f,3);
    sph2.SetFilled(false);
              
    while(!app.MainLoop())
    {
        //~ app.Draw(tr);
        //~ app.Draw(rect);
        //~ app.Draw(rect2);
        
        //~ app.Draw(image3);
        //~ app.Draw(image);
        //~ app.Draw(image2);
        
        //~ for (int i = 0; i < trs.size(); i++)
        //~ {
            //~ app.Draw(trs[i]);
        //~ } 
        
        //~ app.Draw(tr);
        //~ app.Draw(tr3);
        rot += 0.001f;
        //~ oc.Rotate(rot, 0.0f, 0.0f);
        //~ app.Draw(oc); 
        

        //~ sgw::Logger::FLogDebug("rotation: %f",rot);
        sph.Rotate(0.0f, rot, 0.0f);
        app.Draw(sph);
        
        //~ sph2.Rotate(0.0f, -rot, 0.0f);
        //~ app.Draw(sph2);
    }

    return 0;
}
