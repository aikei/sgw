#ifndef SGW_APP_DATA_HPP_INCLUDED
#define SGW_APP_DATA_HPP_INCLUDED

#include <math/math.hpp>
#include <string>

namespace sgw
{

struct AppData
{
	int argc;
	char** argv;
	Vec2 windowPosition;
	Size windowSize;
	std::string title;
	
	AppData()
	{
		windowPosition = Vec2(0,0);
		windowSize = Size(1024, 768);
		title = "Window";
	}
};

}

#endif
