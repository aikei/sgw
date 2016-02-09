#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <sgw/sgw.hpp>
#include <sgw/Prefs.hpp>
#include <stdexcept>
#include <utils/Logger.hpp>

#ifdef SGW_RENDERER_OPENGL
#include <GL/glew.h>
#endif

#include "check.hpp"

int testsSucceeded;
int testsFailed;
int testNumber;


test::Check::Check(sgw::App& app) : m_app(app)
{
    testsSucceeded = 0;
    testsFailed = 0;
    testNumber = 0;
}

void test::Check::RunTests()
{
    sgw::Logger::LogLevel = sgw::Logger::LOG_LEVEL_DEBUG;
    //sgw::Logger::LogLevel = sgw::Logger::LOG_LEVEL_TRACE;
    TestDraw("Draw filled rect test",sgw::BaseShape::SHAPE_RECTANGLE,true,10);
    TestDraw("Draw non-filled rect test",sgw::BaseShape::SHAPE_RECTANGLE,false,10);
    TestDraw("Draw image test",sgw::BaseShape::SHAPE_IMAGE,false,10);
    sgw::Logger::FLogAlways("Tests succeeded: %d | Tests failed: %d\n",testsSucceeded,testsFailed);
}

sgw::Color CreateRandomColor()
{
    sgw::Color clr = rand() % sgw::Color::WHITE;
    return clr;
}

std::unique_ptr<sgw::BaseShape> test::Check::CreateRandomShape(int shapeType)
{
    const sgw::Size& windowSize = m_app.GetAppData().windowSize;
    std::unique_ptr<sgw::BaseShape> shpPtr;
    // get a random x and y, but not 0 and not == screen size-1, 
    // to be able to check outside border of the primitive   
    int sw = windowSize.width;
    int sh = windowSize.height;     
    int x = (rand() % (sw/2))+1;
    int y = (rand() % (sh/2))+1;
    switch(shapeType)
    {        
        case sgw::BaseShape::SHAPE_RECTANGLE:
            {
                int w = rand() % 300;
                int h = rand() % 300;
                if (x+w >= sw-1)
                {
                    w = sw-2-x;
                    sgw::Logger::FLogTrace("%s","x+w >= sw-1\n");
                }
                if (y+h >= sh-1)
                {
                    h = sh-2-y;
                    sgw::Logger::FLogTrace("%s","y+h >= sh-1\n");
                }
                //~ x = 2;
                //~ y = 2;
                //~ w = 5;
                //~ h = 5;
                sgw::Logger::Logger::FLogDebug("x = %d, y = %d, w = %d, h = %d\n",x,y,w,h);
                shpPtr.reset(new sgw::Rect(x,y,w,h));
                shpPtr->SetColor(CreateRandomColor());
            }
            break;
            
        case sgw::BaseShape::SHAPE_IMAGE:
            {
                sgw::Texture* texture = new sgw::Texture("../media/test3.png");
                shpPtr.reset(new sgw::Image(*texture));
                shpPtr->SetPos(x,y);
            }
            break;
        
        default:
            throw std::runtime_error("wrong shape type!");
            break;
    }
    return shpPtr;
}


#ifdef SGW_RENDERER_OPENGL
unsigned int test::Check::CheckPixel(float x, float y)
{
    sgw::AppData appData = m_app.GetAppData();
    unsigned int clr[1];
    glReadPixels(x, appData.windowSize.height-1-y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, clr);
    sgw::Logger::FLogInfo("clr { %.1f, %.1f } = %X\n",x,y,clr[0]);
    return clr[0];
}

bool test::Check::CheckDraw(const sgw::BaseShape& shape)
{
    //first draw shape, then Render it twice - because frame buffer is
    //swapped after render and we need the image in the framebuffer to
    //check it with glReadPixels()
    m_app.Draw(shape);
    m_app.Render();
    m_app.Render();
    
    //get the color of the shape to check screen pixels agaist
    sgw::Color drawColor = shape.GetColor();

    switch(shape.GetType())
    {
        case sgw::BaseShape::SHAPE_RECTANGLE:
        case sgw::BaseShape::SHAPE_IMAGE:
            //we add and subtract 1 from top left and bottom right to
            //make sure that the border is of background color (0x0)
            sgw::Vec2 tl = shape.GetPos();
            tl.x -= 1;
            tl.y -= 1;

            sgw::Vec2 br = shape.GetPos()+shape.GetSize();
            
            //allow some fault tolerance, in pixels, especially for 
            //non-filled shapes, since lines are imprecise
            const int faultTolerance = 4;
            int fails = 0;    
            
            //get integer width and height of the whole area to be checked
            unsigned int wi = br.x-tl.x+1;
            unsigned int hi = br.y-tl.y+1;
            sgw::Logger::FLogTrace("\thi = %d, wi = %d\n",hi,wi);
            float x = tl.x;
            float y = tl.y+shape.GetSize().height+1;
            unsigned int size = wi*hi;
            //printf("\tsize = %d\n",size);
            unsigned int* pixels = new unsigned int[size];
            sgw::AppData appData = m_app.GetAppData();
            unsigned char* imageData = NULL;
            //if there is a texture (for SHAPE_IMAGE), get raw pixel data
            if (shape.GetTexture())
            {
                imageData = shape.GetTexture()->GetRawImageData();
            }
            
            glReadPixels(x, appData.windowSize.height-1-y, wi, hi, GL_RGBA, GL_UNSIGNED_BYTE, pixels);     
            
            for (int i = hi-1; i >= 0; i--)
            {
                sgw::Logger::FLogInfo("%s","\n\n");
                for (int j = 0; j < wi; j++)
                {
                    unsigned int px = pixels[j+(i*wi)];
                    px = sgw::Utils::ToBigEndian(px);             
                    sgw::Logger::FLogTrace("{ %d, %d } = %X;\n",int(tl.x+i),int(tl.y+j),px);
                    if (i == 0 || j == 0 || i == hi-1 || j == wi-1)
                    {
                        if (px != 0)
                        {
                            fails++;
                            if (fails >= faultTolerance)
                            {
                                sgw::Logger::FLogAlways("Test failed - outside at %d, %d is %X!\n",j,i,px);
                                return false;                            
                            }
                        }
                    }
                    else
                    {
                        //if there is texture data, get the color and check
                        //that the pixel has been drawn where it belongs
                        if (imageData)
                        {
                            unsigned char clr[4];
                            unsigned int xx = (j-1)*4;
                            unsigned int yy = i-1;
                            unsigned int n = xx+yy*(wi-2)*4;
                            
                            for (unsigned int c = 0; c < 4; c++)
                            {
                                clr[c] = imageData[n+c];
                            }                            
                            drawColor = sgw::Color(clr[0],clr[1],clr[2],clr[3]);
                            sgw::Logger::FLogInfo("n = %u,xx/yy { %u / %u }, clr = %d %d %d %d drawClr = %X | ",
                            n,xx/4,yy,clr[0],clr[1],clr[2],clr[3],drawColor.GetHexColor());                          
                        }
                        
                        if (shape.IsFilled())
                        {
                            if (!drawColor.AlmostEqual(px))
                            {
                                fails++;
                                if (fails >= faultTolerance)
                                {                                                
                                    sgw::Logger::FLogAlways("Test failed - filled rect"
                                        " inside color at %u, %u is not"
                                        " %X but %X!\n",
                                        j,i,drawColor.GetHexColor(),px);
                                    return false;
                                }
                            }
                        }
                        else
                        {
                            if (i == 1 || j == 1 || i == hi-2 || j == wi-2)
                            {
                                if (!drawColor.AlmostEqual(px))
                                {
                                    fails++;
                                    if (fails >= faultTolerance)
                                    {                            
                                        sgw::Logger::FLogAlways("Test failed - non-filled rect border at %d, %d is %X!\n",j,i,px);
                                        return false;
                                    }
                                }
                            }
                            else
                            {
                                if (px != 0)
                                {
                                    fails++;
                                    if (fails >= faultTolerance)
                                    {
                                        sgw::Logger::FLogAlways("Test failed - non-filled rect inside color at %d, %d is %X!\n",j,i,px);
                                        return false; 
                                    }                           
                                }
                            }
                        }
                    }
                }
            }
            delete[] pixels;
            break;            
    }
    
    return true;
}
#endif

bool test::Check::TestDraw(const char* testName, int shapeType, bool filled, int numRuns)
{
    testNumber++;
    sgw::Logger::FLogAlways("%d. %s.\n",testNumber,testName);
    bool success = true;
    sgw::Logger::FLogAlways("\tRun: ");

    for (int i = 0; i < numRuns; i++)
    {
        sgw::Logger::FLogAlways("%d, ",i);
        std::unique_ptr<sgw::BaseShape> shapePtr = CreateRandomShape(shapeType);
        shapePtr->SetFilled((bool)filled);
        if (!CheckDraw(*shapePtr))
        {
            sgw::Logger::FLogAlways("\t%s failed\n\n",testName);
            testsFailed++;
            return false;
        }
    }
    testsSucceeded++;
    sgw::Logger::FLogAlways("\n\t%s succeeded\n\n",testName);
    
    return true;
}
