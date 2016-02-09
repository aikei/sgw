#ifndef SGW_H_INCLUDED
#define SGW_H_INCLUDED

#include "App.hpp"
#include "AppData.hpp"
#include "Prefs.hpp"
#include "Color.hpp"
#include "Texture.hpp"
#include "geom/AllShapes.hpp"
#include "geom/Size.hpp"
#include "geom/Vec2.hpp"
#include "geom/Vec3.hpp"

/**
 * \mainpage SGW - a simple graphics wrapper
 * 
 * SGW is a simple graphics wrapper library for C++.
 * 
 * # Usage
 * Simplest use case:
 * \code
 * #include <sgw/sgw.hpp>
 * int main(int argc, char** argv)
 * {
 *     sgw::App app(800,600);
 *     sgw::Texture texture("test.png");
 *     sgw::Image image(texture);
 *     while(!app.MainLoop())
 *     {
 *         app.Draw(image);            
 *     }
 * }
 * \endcode
 * 
 * 
 */


#endif
