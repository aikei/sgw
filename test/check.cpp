#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <sgw/Prefs.hpp>
#include <stdexcept>
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
    TestDraw("Draw rect test",sgw::BaseShape::SHAPE_RECTANGLE,10);
    printf("Tests succeeded: %d | Tests failed: %d\n",testsSucceeded,testsFailed);
}

std::unique_ptr<sgw::BaseShape> test::Check::CreateRandomShape(int shapeType)
{
    const sgw::Size& windowSize = m_app.GetAppData().windowSize;
    switch(shapeType)
    {
        case sgw::BaseShape::SHAPE_RECTANGLE:
            // get a random x and y, but not 0 and not == screen size-1, 
            // to be able to check outside border of the primitive
            int sw = windowSize.width;
            int sh = windowSize.height;
            int x = (rand() % (sw-10))+1;
            int y = (rand() % (sh-10))+1;
            int x2 = x+(rand() % 300);
            int y2 = y+(rand() % 300);
            if (x2 >= sw-1)
                x2 = sw-2;
            if (y2 >= sh-1)
                y2 = sh-2;
                
            std::unique_ptr<sgw::BaseShape> rectPtr (new sgw::Rect(x,y,x2,y2));
            return rectPtr;
            break;           
    }
    
    throw std::runtime_error("wrong shape type!");
}


#ifdef SGW_RENDERER_OPENGL
unsigned int test::Check::CheckPixel(float x, float y)
{
    sgw::AppData appData = m_app.GetAppData();
    unsigned int clr[1];
    glReadPixels(x, appData.windowSize.height-1-y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, clr);
    printf("clr { %.1f, %.1f } = %X\n",x,y,clr[0]);
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
    
    int shapeType = shape.GetType();
    switch(shapeType)
    {
        case sgw::BaseShape::SHAPE_RECTANGLE:
            //we add and subtract 1 from top left and bottom right to check that
            //the border is of background color
            const sgw::Rect& rect = static_cast<const sgw::Rect&>(shape);
            sgw::Vec2 tl = rect.GetTopLeft();
            tl.x -= 1;
            tl.y -= 1;

            sgw::Vec2 br = rect.GetBottomRight();
            br.x += 1;
            br.y += 1;
            
            //allow some fault tolerance, in pixels
            const int faultTolerance = 10;
            int fails = 0;    
            
            float w = br.x - tl.x+1;
            unsigned int wi = w;
            float h = br.y - tl.y+1;
            float x = tl.x;
            float y = br.y;
            unsigned int size = w*h;
            //printf("\tsize = %d\n",size);
            unsigned int* pixels = new unsigned int[size];
            sgw::AppData appData = m_app.GetAppData();
                
            glReadPixels(x, appData.windowSize.height-1-y, w, h, GL_RGBA, GL_UNSIGNED_BYTE, pixels);     
            
            for (int i = 0; i < w; i++)
            {
                for (int j = h-1; j >= 0; j--)
                {
                    unsigned int px = pixels[i+(j*wi)];
                    //printf("{ %d, %d } = %X;\n",int(tl.x+i),int(tl.y+j),px);
                    if (i == 0 || j == 0 || i == int(w-1) || j == int(h-1))
                    {
                        if (px != 0)
                        {
                            fails++;
                            if (fails >= faultTolerance)
                            {
                                printf("Test failed - outside not black!\n");
                                return false;                            
                            }
                        }
                    }
                    else
                    {
                        if (rect.IsFilled())
                        {
                            if (px != 0xFFFFFFFF)
                            {
                                fails++;
                                if (fails >= faultTolerance)
                                {                                                
                                    printf("Test failed - filled rect inside color not white!\n");
                                    return false;
                                }
                            }
                        }
                        else
                        {
                            if (i == 1 || j == 1 || i == int(w-2) || j == int(h-2))
                            {
                                if (px != 0xFFFFFFFF)
                                {
                                    fails++;
                                    if (fails >= faultTolerance)
                                    {                            
                                        printf("Test failed - non-filled rect border not white!\n");
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
                                        printf("Test failed - non-filled rect inside not black!\n");
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

void test::Check::TestDraw(const char* testName, int shapeType, int numRuns)
{
    testNumber++;
    //test both filled (t == 1) and non-filled (t == 0) shapes
    for (int filled = 0; filled < 2; filled++)
    {
        if (filled)
            printf("%d. %s\n\tOutline shape test.\n\tRun: ",testNumber,testName);
        else
            printf("\n\tFilled shape test.\n\tRun: ");
            
        for (int i = 0; i < numRuns; i++)
        {
            printf("%d, ",i);
            std::unique_ptr<sgw::BaseShape> shapePtr = CreateRandomShape(shapeType);
            shapePtr->SetFilled((bool)filled);
            if (!CheckDraw(*shapePtr))
            {
                printf("\t%s failed\n\n",testName);
                testsFailed++;
                return;
            }
        }
    }
 
    printf("\n\t%s succeeded\n\n",testName);
    testsSucceeded++;    
}
