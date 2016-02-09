#ifndef SGW_COLOR_HPP_INCLUDED
#define SGW_COLOR_HPP_INCLUDED

#include <cstring>
#include <stddef.h> 
#include <utils/utils.hpp>
#include <cstdio>
#include <cstdlib>

namespace sgw
{
    static float const BITS_PER_CHANNEL = 256.0f;

    struct Color
    {
        ///red component
        unsigned char r;
        ///green component
        unsigned char g;
        ///blue component
        unsigned char b;
        ///alpha component
        unsigned char a;
        
        /** \brief Initilizes color from hex color
         * 
         * Some examples: 0xFF0000 - red, 0x00FF00FF - green, etc.
         * WHITE (0xFFFFFF) is the default color.
         * Note that fourth byte does not matter and alpha will be set to
         * 255 anyway. You need to set it manually, if you wish, but you
         * usually don't need to do that and should set opacity of the
         * shape you are about to draw.
         * @param color - the hex color
         */
        Color(unsigned int color=Color::WHITE)
        {
            color = Utils::ToBigEndian(color);
            memcpy(this,&color,4);
            a = 255;
        }
        
        /** \brief Initilizes color from RGBA components
         * 
         * All components must be in range [0.0, 1.0]
         * @param r - red component
         * @param g - green component
         * @param b - blue component
         * @param a - alpha component (defaults to 1.0f)
         */        
        Color(float r, float g, float b, float a = 1.0f)
        {
            this->r = static_cast<unsigned char>(r*(BITS_PER_CHANNEL-1));
            this->g = static_cast<unsigned char>(g*(BITS_PER_CHANNEL-1));
            this->b = static_cast<unsigned char>(b*(BITS_PER_CHANNEL-1));
            this->a = static_cast<unsigned char>(a*(BITS_PER_CHANNEL-1));
        }
        
        Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255)
        {
            this->r = r;
            this->g = g;
            this->b = b;
            this->a = a;
        }
        
        ///Copy constructor
        Color(const Color& clr)
        {
            memcpy(this,&clr,4);
        }        
        
        const Color& operator=(const Color& right)
        {
            memcpy(this,&right,4);
        }
        
        bool operator==(unsigned int right) const
        {
            //~ Color oth(right);
            //~ return (r == oth.r && g == oth.g && b == oth.b);
            return GetHexColor() == right;
        }
        
        bool operator!=(unsigned int right) const
        {
            Color oth(right);
            return (r != oth.r || g != oth.g || b != oth.b);
        }
        
        bool AlmostEqual(const Color& color, unsigned char tolerance=0, bool considerAlpha=false)
        {
            if (abs(r-color.r) <= tolerance && 
                abs(g-color.g) <= tolerance &&
                abs(b-color.b) <= tolerance &&
                (!considerAlpha || abs(a-color.a) <= tolerance))
            {
                return true;
            }
            printf("almost equal false: %d %d %d %d and %d %d %d %d\n",r,g,b,a,color.r,color.g,color.b,color.a);
            return false;
        }
        
        inline unsigned int GetHexColor() const
        {
            unsigned int clr = 0;
            memcpy(&clr,this,4);
            return Utils::ToHostEndian(clr);
        }
        
        Color operator*(const float scalar) const
        {
            Color retClr = *this;
            retClr.r = (float)retClr.r*scalar;
            retClr.g = (float)retClr.g*scalar;
            retClr.b = (float)retClr.b*scalar;
            retClr.a = (float)retClr.a*scalar;
            return retClr;
        }
                
        ///static hex constant for red color
        static const unsigned int RED = 0xFF0000FF;     
        ///static hex constant for green color
        static const unsigned int GREEN = 0x00FF00FF;   
        ///static hex constant for blue color
        static const unsigned int BLUE = 0x0000FFFF;  
        ///static hex constant for white color
        static const unsigned int WHITE = 0xFFFFFFFF;
        ///static hex constant for black color
        static const unsigned int BLACK = 0x000000FF;
        ///static hex constant for yellow color
        static const unsigned int YELLOW = 0xFFFF00FF;
    };
    
    static bool operator==(unsigned int left,const Color& right)
    {
        //~ Color oth(left);
        //~ return (right.r == oth.r && right.g == oth.g && right.b == oth.b && right.a == oth.a);
        return right.GetHexColor() == left;        
    }
    
    static bool operator!=(unsigned int left,const Color& right)
    {
        //~ Color oth(left);
        //~ return (right.r != oth.r || right.g != oth.g || right.b != oth.b || right.a != oth.a);
        return right.GetHexColor() != left;
    }    
}


#endif
