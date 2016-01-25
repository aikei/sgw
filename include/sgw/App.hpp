#ifndef SGW_APP_HPP_INCLUDED
#define SGW_APP_HPP_INCLUDED

#include "IApp.hpp"

namespace sgw
{
	namespace App
	{
		IApp* Create(float windowWidth, float windowHeight);
		IApp* Create(const AppData& _appData);
	}
}

#endif
