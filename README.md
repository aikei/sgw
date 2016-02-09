# sgw
Sgw is a simple graphics wrapper for OpenGL.

## Compiling
To compile, you need cmake 3.1 and all the dependencies. The dependencies
are:
- glfw
- glew
- libpng
- zlib

## Usage
Simplest use case:
```C++
#include <sgw/sgw.hpp>
int main(int argc, char** argv)
{
    sgw::App app(800,600);
    sgw::Texture texture("test.png");
    sgw::Image image(texture);
    while(!app.MainLoop())
    {
        app.Draw(image);            
    }
}
```
