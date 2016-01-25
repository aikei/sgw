#ifndef SGW_COLOR_HPP_INCLUDED
#define SGW_COLOR_HPP_INCLUDED

namespace sgw
{
	static float BITS_PER_CHANNEL = 256;
	
	struct Color
	{
		unsigned char r;
		unsigned char g;
		unsigned char b;
		unsigned char a;
		
		Color() { r = g = b = a = 0; }
		
		Color(unsigned char _r, unsigned char _g, unsigned char _b, unsigned char _a)
		{
			r = _r;
			g = _g;
			b = _b;
			a = _a;
		}
		
		Color(float _r, float _g, float _b, float _a)
		{
			r = static_cast<unsigned char>(_r*(BITS_PER_CHANNEL-1));
			g = static_cast<unsigned char>(_g*(BITS_PER_CHANNEL-1));
			b = static_cast<unsigned char>(_b*(BITS_PER_CHANNEL-1));
			a = static_cast<unsigned char>(_a*(BITS_PER_CHANNEL-1));
		}
	};
}

#endif
